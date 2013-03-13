/**************************************************************************
 *  Copyright 2013 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "kovan/ardrone.hpp"
#include "kovan/thread.hpp"
#include "kovan/util.h"
#include "ardrone_constants_p.hpp"
	 
#ifdef WIN32
	 #error AR.Drone library doesn't work on Windows.
#endif

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
	
	uint8_t data[];
} __attribute__((packed));

struct navdata_t {
	// Always set to NAVDATA_HEADER
	uint32_t header;

	// Bit mask built from def_ardrone_state_mask_t
	uint32_t ardrone_state;

	// Sequence number, incremented for each sent packet
	uint32_t sequence;
	uint32_t vision_defined;

	navdata_option_t options[];
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
	while(current + times >= m_current) msleep(10);
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
	
	void control();
	void sendMagic();
	
	void setOwner(const char *const mac);
	
	bool isControlWatchdog() const;
	bool isLowBatt() const;
	
	
	void resetControlWatchdog();
	void sendCommWatchdog();
	
	std::string currentConfig();
	
	const navdata_t *latestNavdata() const;
	bool writeNavdata(const char *const path) const;
	
private:
	void pushCommand(const char *const command);
	void popCommand();
	
	void oneTimeCommand(const char *const command);
	
	bool update();
	bool fetchNavdata();
	
	void __enumerateOptions();
	
	sockaddr_in m_addr;
	int m_fd;
	Mutex m_mutex;
	Mutex m_control;
	std::stack<DroneCommand> m_commandStack;
	DroneSequencer m_seq;
	bool m_stop;
	unsigned char m_navdata[65507];
};

DroneController::DroneController()
	: m_fd(-1)
{
	memset(m_navdata, 0, sizeof(m_navdata));
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
	if(m_fd < 0) return true;
	
	sockaddr_in navport = m_addr;
	navport.sin_port = htons(ARDRONE_NAVDATA_PORT);
	
	const static char dummy[4] = { 0x01, 0x00, 0x00, 0x00 };
	if(sendto(m_fd, dummy, sizeof(dummy), 0,
		(const sockaddr *)&navport, sizeof(navport)) != sizeof(dummy)) {
		perror("DroneController::requestNavdata -> sendto");
		return false;
	}
	
	return true;
}

void DroneController::config(const char *const cmd, const char *const value)
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,\"%s\",\"%s\",\"%s\"\r", ARDRONE_AT_CONFIG_IDS,
		ARDRONE_SESSION_ID,ARDRONE_USER_ID, ARDRONE_APPLICATION_ID);
	oneTimeCommand(command);
	
	sprintf(command, "%s%%u,\"%s\",\"%s\"\r", ARDRONE_AT_CONFIG, cmd, value);
	oneTimeCommand(command);
}
	
void DroneController::run()
{
	unsigned long ticks = 0;
	while(!m_stop) {
		if(ticks % 100 == 0) {
			// sendCommWatchdog();
			requestNavdata();
		}
		m_mutex.lock();
		bool success = true;
		
		// Slower update sub-loop
		if(++ticks % 11 == 0) {
			__enumerateOptions();
			success &= update();
		}
		success &= fetchNavdata();
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

void DroneController::control()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_CTRL, 0);
	oneTimeCommand(command);
}

void DroneController::sendMagic()
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,%u\r", ARDRONE_AT_PMODE, 2);
	oneTimeCommand(command);
	msleep(100);
    sprintf(command, "%s%%u,%u,%u,%u,%u\r", ARDRONE_AT_MISC, 2, 20, 2000, 3000);
	oneTimeCommand(command);
	msleep(100);
}

void DroneController::setOwner(const char *const mac)
{
	config(ARDRONE_NETWORK_OWNER_MAC, mac);
}

bool DroneController::isControlWatchdog() const
{
	const navdata_t *const latest = latestNavdata();
	if(!latest) return false;
	
	return latest->ardrone_state & ARDRONE_CTRL_WATCHDOG_MASK;
}

bool DroneController::isLowBatt() const
{
	const navdata_t *const latest = latestNavdata();
	if(!latest) return false;
	
	return latest->ardrone_state & ARDRONE_VBAT_LOW;
}

void DroneController::resetControlWatchdog()
{
	// control(ARDRONE_ACK_CONTROL_MODE);
}

void DroneController::sendCommWatchdog()
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

const navdata_t *DroneController::latestNavdata() const
{
	const navdata_t *const ret = reinterpret_cast<const navdata_t *>(m_navdata);
	if(!ret || ret->header != ARDRONE_NAVDATA_HEADER) return 0;
	return ret;
}

bool DroneController::writeNavdata(const char *const path) const
{
	FILE *fp = fopen(path, "w");
	if(!fp) return false;
	fwrite(m_navdata, 1, sizeof(m_navdata), fp);
	fclose(fp);
	return true;
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
	m_seq.wait(2);
	popCommand();
}

bool DroneController::fetchNavdata()
{
	if(m_fd < 0) return true;
	
	ssize_t readLength = 0;
	char data[sizeof(m_navdata)];
	if((readLength = recv(m_fd, data, sizeof(data), 0)) < 0 && errno != EAGAIN) {
		perror("recvfrom");
		return false;
	}
	if(readLength < 0) return true;
	std::cout << "readLength: " << readLength << std::endl;
	
	memcpy(m_navdata, data, sizeof(m_navdata));
	return true;
}

void DroneController::__enumerateOptions()
{
	const navdata_t *const navdata = latestNavdata();
	if(!navdata || navdata->header != ARDRONE_NAVDATA_HEADER) return;
	
	const navdata_option_t *option = navdata->options;
	// std::cout << "Got tag: " << (int)option->tag << std::endl;
	// std::cout << "\tsize:  " << (int)option->size << " (navdata_demo_t size: " << sizeof(navdata_demo_t) << ")" << std::endl;
	for(int i = 0; i < 3; option += option->size, ++i) {
		std::cout << "Got tag: " << (int)option->tag << std::endl;
		std::cout << "\tsize:  " << (int)option->size << " (navdata_demo_t size: " << sizeof(navdata_demo_t) << ")" << std::endl;
	}
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
	memset(&bindAddr, 0, sizeof(bindAddr));
	bindAddr.sin_family = AF_INET;
	bindAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bindAddr.sin_port = htons(ARDRONE_NAVDATA_PORT);
	if(bind(m_fd, (struct sockaddr *)&bindAddr, sizeof(bindAddr)) < 0) {
		perror("bind");
		close(m_fd);
		return false;
	}
	
	const int yes = 1;
	setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
	
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
	m_controller->sendMagic();
	m_controller->config(ARDRONE_NAVDATA_DEMO, ARDRONE_TRUE);
	m_controller->control();
	
	// Set ourself as the owner
	// m_controller->config("network:ssid_single_player","myArdroneNetwork");
	// m_controller->setOwner("14:10:9f:e3:6c:fd");
	
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
	const navdata_t *const navdata = m_controller->latestNavdata();
	if(!navdata) return ARDrone::Disconnected;
	
	static int i = 0;
	char buffer[32];
	sprintf(buffer, "navdata_%d.bin", i++);
	m_controller->writeNavdata(buffer);
	
	if(navdata->ardrone_state & ARDRONE_FLY_MASK) return ARDrone::Flying;
	return ARDrone::Landed;
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