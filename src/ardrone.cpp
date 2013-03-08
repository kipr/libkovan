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
#include <iostream>
#include <cstring>
#include <string>
#include <errno.h>
#include <fcntl.h>

#include <stack>

#include <stdint.h>

struct navdata_option_t
{
	// Navdata block ('option') identifier
	uint16_t tag;
	
	// set this to the size of this structure
	uint16_t size;
	
	uint8_t data[1];
} __attribute__((packed));

struct navdata_t {
	// Always set to NAVDATA_HEADER
	uint32_t header;

	// Bit mask built from def_ardrone_state_mask_t
	uint32_t ardrone_state;

	// Sequence number, incremented for each sent packet
	uint32_t sequence;
	bool vision_defined;

	navdata_option_t options[1];
} __attribute__((packed));

struct navdata_demo_t
{
	// Flying state (landed, flying, hovering, etc.) defined in CTRL_STATES enum.
	uint32_t ctrl_state;
	
	// battery voltage filtered (mV)
	uint32_t vbat_flying_percentage;

	// UAV's pitch in milli-degrees
	float pitch;
	
	// UAV's roll in milli-degrees
	float roll;
	
	// UAV's yaw in milli-degrees
	float yaw;

	// UAV's altitude in centimeters
	int32_t altitude;

	// UAV's estimated linear velocity
	float vx;
	
	// UAV's estimated linear velocity
	float vy;
	
	// UAV's estimated linear velocity
	float vz;

	uint32_t num_frames;
} __attribute__((packed));

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
	
	void setup(const sockaddr_in &addr, int fd);
	void invalidate();
	
	void flatTrim();
	
	void land(const bool emergency = false);
	void takeoff();
	
	void move(const float x, const float y, const float z, const float yaw);
	
	bool requestNavdata();
	void config(const char *const cmd, const char *const value);
	
	void run();
	
	void stop();
	bool isStopped() const;
	
	void control(const unsigned param);
	
	void setOwner(const char *const mac);
	
	void resetWatchdog();
	
	std::string currentConfig();
	
	navdata_demo_t latestNavdata() const;
	
private:
	void pushCommand(const char *const command);
	void popCommand();
	
	void oneTimeCommand(const char *const command);
	
	bool update();
	bool fetchNavdata();
	
	sockaddr_in m_addr;
	int m_fd;
	Mutex m_mutex;
	std::stack<DroneCommand> m_commandStack;
	DroneSequencer m_seq;
	bool m_stop;
};

DroneController::DroneController()
	: m_fd(-1)
{
}

DroneController::~DroneController()
{
}

void DroneController::setup(const sockaddr_in &addr, int fd)
{
	m_addr = addr;
	m_fd = fd;
}

void DroneController::invalidate()
{
	m_mutex.lock();
	m_fd = -1;
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

bool DroneController::requestNavdata()
{
	sockaddr_in si_other;
	memset(&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(ARDRONE_AT_PORT);
	if(inet_aton("192.168.1.1", &si_other.sin_addr) <= 0) {
		perror("inet_aton");
		return false;
	}
	
	const char *const dummy = "braden";
	if(sendto(m_fd, dummy, strlen(dummy), 0,
		(const sockaddr *)&si_other, sizeof(si_other)) < 0) {
		perror("DroneController::requestNavdata -> sendto");
		return false;
	}
	
	return true;
}

void DroneController::config(const char *const cmd, const char *const value)
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,\"%s\",\"%s\"\r", ARDRONE_AT_CONFIG, cmd, value);
	printf("sending command: %s\n", command);
	oneTimeCommand(command);
}
	
void DroneController::run()
{
	unsigned long ticks = 0;
	while(!m_stop) {
		m_mutex.lock();
		bool success = true;
		if(++ticks % 11 == 0) success &= update();
		success &= fetchNavdata();
		requestNavdata();
		m_mutex.unlock();
		if(!success) break;
		msleep(3);
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

void DroneController::control(const unsigned param)
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u,%u\r", ARDRONE_AT_CTRL, param, 0);
	oneTimeCommand(command);
}

void DroneController::setOwner(const char *const mac)
{
	config(ARDRONE_NETWORK_OWNER_MAC, mac);
}

void DroneController::resetWatchdog()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u\r", ARDRONE_AT_COMWDG);
	oneTimeCommand(command);
}

std::string DroneController::currentConfig()
{
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd < 0) return std::string();
	
	sockaddr_in addr = m_addr;
	addr.sin_port = htons(5559);
	std::cout << "connecting" << std::endl;
	
	if(connect(fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("connect");
		close(fd);
		return std::string();
	}
	
	std::cout << "sending" << std::endl;
	
	
	
	std::cout << "recving" << std::endl;
	std::string ret;
	char buffer[128];
	while(recv(fd, buffer, 128, 0) < 0 && errno == EAGAIN) {
		std::cout << buffer << std::endl;
		ret += buffer;
	}
	ret += buffer;
	if(errno != EAGAIN) {
		perror("recv");
	}
	
	close(fd);
	
	return ret;
}

navdata_demo_t DroneController::latestNavdata() const
{
	return navdata_demo_t();
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

bool DroneController::fetchNavdata()
{
	if(m_fd < 0) return true;
	
	char data[512];
	ssize_t readLength = 0;
	if((readLength = recv(m_fd, &data, 512, 0)) < 0 && errno != EAGAIN) {
		perror("recvfrom");
		return false;
	}
	std::cout << "Read length: " << readLength << std::endl;
	if(readLength > 0) {
		std::cout << "Read length: " << readLength << " navdata_t size: " << sizeof(navdata_t) << std::endl;
		std::cout << data << std::endl;
	}
	return true;
}

bool DroneController::update()
{
	// Nothing to do.
	if(m_commandStack.empty() || m_fd < 0) return true;
	
	char realCommand[ARDRONE_MAX_CMD_LENGTH];
	sprintf(realCommand, m_commandStack.top().data, m_seq.next());
	printf("Sending %s\n", realCommand);
	if(sendto(m_fd, realCommand, strlen(realCommand), 0,
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
	m_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(m_fd < 0) return false;
	
	const int flags = fcntl(m_fd, F_GETFL);
	fcntl(m_fd, F_SETFL, flags | O_NONBLOCK);
	
	sockaddr_in bindAddr;
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindAddr.sin_port = htons(ARDRONE_NAVDATA_PORT);
	if(bind(m_fd, (struct sockaddr *)&bindAddr, sizeof(bindAddr)) < 0) {
		perror("bind");
		close(m_fd);
		return false;
	}
	
	sockaddr_in si_other;
	memset(&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(ARDRONE_AT_PORT);
	if(inet_aton(ip, &si_other.sin_addr) == 0) {
		perror("inet_aton");
		close(m_fd);
		return false;
	}
		
	m_controller->setup(si_other, m_fd);
	
	// m_controller->resetWatchdog();
	
	m_controller->requestNavdata();
	// m_controller->config(ARDRONE_NAVDATA_DEMO, ARDRONE_TRUE);
	// m_controller->control(ARDRONE_ACK_CONTROL_MODE);
	
	// Set ourself as the owner
	// m_controller->config("network:ssid_single_player","myArdroneNetwork");
	// m_controller->control(ARDRONE_ACK_CONTROL_MODE);
	// m_controller->setOwner("14:10:9f:e3:6c:fd");
	// m_controller->control(ARDRONE_ACK_CONTROL_MODE);
	
	
	// std::cout << m_controller->currentConfig() << std::endl;
	
	return true;
}

void ARDrone::disconnect()
{
	// Already disconnected?
	if(m_fd < 0) return;
	
	m_controller->land(true);
	msleep(100);
	// while(state() != ARDrone::Landed) msleep(100);
	
	m_controller->invalidate();
	
	close(m_fd);
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
	, m_fd(-1)
{
	m_controller->start();
}