#include "kovan/ardrone.hpp"
#include "kovan/thread.hpp"
#include "kovan/util.h"
#include "ardrone_constants_p.hpp"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <stack>

class DroneSequencer
{
public:
	typedef unsigned sequence_t;
	
	DroneSequencer();
	void wait(const unsigned times = 1);
	DroneSequencer::sequence_t next();
	void reset();
	
private:
	sequence_t m_current;
};

DroneSequencer::DroneSequencer()
{
	reset();
}

void DroneSequencer::wait(const unsigned times)
{
	sequence_t current = m_current;
	while(current + times < m_current) msleep(10);
}

DroneSequencer::sequence_t DroneSequencer::next()
{
	return ++m_current;
}

void DroneSequencer::reset()
{
	m_current = 0;
}

struct DroneCommand
{
	DroneCommand();
	
	char data[ARDRONE_MAX_CMD_LENGTH];
};

DroneCommand::DroneCommand()
{
	memset(data, 0, ARDRONE_MAX_CMD_LENGTH);
}

class DroneController : public Thread
{
public:
	DroneController();
	~DroneController();
	
	void setup(const sockaddr_in &addr, int controlFd);
	void invalidate();
	
	void flatTrim();
	
	void land(const bool emergency = false);
	void takeoff();
	
	void move(const float x, const float y, const float z, const float yaw);
	
	void run();
	
	void stop();
	bool isStopped() const;
	
private:
	void pushCommand(const char *const command);
	void popCommand();
	
	void oneTimeCommand(const char *const command);
	
	bool update();
	
	sockaddr_in m_addr;
	int m_controlFd;
	Mutex m_mutex;
	std::stack<DroneCommand> m_commandStack;
	DroneSequencer m_seq;
	bool m_stop;
};

DroneController::DroneController()
	: m_controlFd(-1)
{
}

DroneController::~DroneController()
{
}

void DroneController::setup(const sockaddr_in &addr, int controlFd)
{
	m_addr = addr;
	m_controlFd = controlFd;
}

void DroneController::invalidate()
{
	m_mutex.lock();
	m_controlFd = -1;
	m_mutex.unlock();
}

void DroneController::flatTrim()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u\r", ARDRONE_AT_FTRIM);
	oneTimeCommand(command);
}

void DroneController::land(const bool emergency)
{
	popCommand();
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_REF, 0x11540000 | ((emergency ? 1 : 0) << 8));
	pushCommand(command);
}

void DroneController::takeoff()
{
	popCommand();
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_REF, 0x11540200);
	pushCommand(command);
}

void DroneController::move(const float x, const float y, const float z, const float yaw)
{
	unsigned ix = *(unsigned *)&x;
	unsigned iy = *(unsigned *)&y;
	unsigned iz = *(unsigned *)&z;
	unsigned iyaw = *(unsigned *)&yaw;
	
	const bool hoverMode = x == 0.0f && y == 0.0f
		&& z == 0.0f && yaw == 0.0f;
	
	popCommand();
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u,%u,%u,%u,%u\r", ARDRONE_AT_PCMD, hoverMode ? 0 : 1,
		ix, iy, iz, iyaw);
	pushCommand(command);
}
	
void DroneController::run()
{
	while(!m_stop) {
		m_mutex.lock();
		bool success = update();
		m_mutex.unlock();
		if(!success) break;
		msleep(33); // ~30 FPS
	}
	m_stop = false;
	m_seq.reset();
}

void DroneController::stop()
{
	m_mutex.lock();
	m_stop = true;
	m_mutex.unlock();
}

bool DroneController::isStopped() const
{
	return m_stop;
}

void DroneController::pushCommand(const char *const command)
{
	DroneCommand container;
	strncpy(container.data, command, ARDRONE_MAX_CMD_LENGTH);
	
	m_mutex.lock();
	m_commandStack.push(container);
	m_mutex.unlock();
}

void DroneController::popCommand()
{
	m_mutex.lock();
	if(!m_commandStack.empty()) m_commandStack.pop();
	m_mutex.unlock();
}

void DroneController::oneTimeCommand(const char *const command)
{
	pushCommand(command);
	m_seq.wait(10);
	popCommand();
}

bool DroneController::update()
{
	// Nothing to do.
	if(m_commandStack.empty() || m_controlFd < 0) return true;
	
	char realCommand[ARDRONE_MAX_CMD_LENGTH];
	sprintf(realCommand, m_commandStack.top().data, m_seq.next());
	printf("Sending %s\n", realCommand);
	if(sendto(m_controlFd, realCommand, strlen(realCommand), 0,
		(const sockaddr *)&m_addr, sizeof(m_addr)) < 0) {
		perror("DroneController::run -> sendto");
		return false;
	}
	
	return true;
}

ARDrone::~ARDrone()
{
	disconnect();
}
	
bool ARDrone::connect(const char *ip)
{
	m_controlFd = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_controlFd < 0) return false;
	
	sockaddr_in si_other;
	memset(&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(ARDRONE_AT_PORT);
	if(inet_aton(ip, &si_other.sin_addr) == 0) {
		perror("inet_aton");
		close(m_controlFd);
		return false;
	}
	
	m_controller->setup(si_other, m_controlFd);
	
	return true;
}

void ARDrone::disconnect()
{
	// Already disconnected?
	if(m_controlFd < 0) return;
	
	m_controller->land(true);
	msleep(100);
	// while(state() != ARDrone::Landed) msleep(100);
	
	m_controller->invalidate();
	
	close(m_controlFd);
}

void ARDrone::flatTrim()
{
	m_controller->flatTrim();
}

void ARDrone::takeoff()
{
	m_controller->takeoff();
}

void ARDrone::land()
{
	m_controller->land();
}

void ARDrone::hover()
{
	m_controller->move(0.0f, 0.0f, 0.0f, 0.0f);
}

void ARDrone::move(const float x, const float y, const float z, const float yaw)
{
	m_controller->move(x, y, z, yaw);
}

ARDrone::State ARDrone::state() const
{
	// TODO: Stub
	return ARDrone::Disconnected;
}
	
ARDrone *ARDrone::instance()
{
	static ARDrone s_instance;
	return &s_instance;
}

ARDrone::ARDrone()
	: m_controller(new DroneController)
	, m_controlFd(-1)
{
	m_controller->start();
}