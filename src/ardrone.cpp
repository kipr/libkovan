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
#include "kovan/socket.hpp"
#include "kovan/util.h"
#include "ardrone_constants_p.hpp"
#include "uvlc_video_decoder_p.hpp"

#include <opencv2/opencv.hpp>

#include <stdio.h>

#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <string>
#include <errno.h>

#include <stack>

#include <stdint.h>

#define ARDRONE_DEBUG

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

struct navdata_mat3_t
{
	float _00;
	float _01;
	float _02;
	float _10;
	float _11;
	float _12;
	float _20;
	float _21;
	float _22;
} __attribute__((packed));

struct navdata_vec3_t
{
	float _0;
	float _1;
	float _2;
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
	navdata_vec3_t velocity;

	uint32_t frameIndex;
	
	navdata_mat3_t detection_cam_rotation;
	navdata_vec3_t detection_cam_translation;
	uint32_t tagIndex;
	uint32_t detectionType;
	
	navdata_mat3_t drone_cam_rotation;
	navdata_vec3_t drone_cam_translation;
} __attribute__((packed));

struct navdata_raw_t
{
	uint16_t accelX;
	uint16_t accelY;
	uint16_t accelZ;
	
	uint16_t gyroX;
	uint16_t gyroY;
	uint16_t gyroZ;
	
	int16_t gyroX_110;
	int16_t gyroY_110;
	
	uint32_t batteryMilliVolt;
	
	uint16_t usEchoStart;
	uint16_t usEchoEnd;
	uint16_t usEchoAssociation;
	uint16_t usEchoDistance;
	
	uint16_t usCurveTime;
	uint16_t usCurveValue;
	uint16_t usCurveRef;
	
	uint16_t echoFlagIni;
	uint16_t echoNum;
	uint16_t echoSum;
        
	int32_t altTemp;
} __attribute__((packed));

struct navdata_phys_t
{
	 float accelTemp;
	 uint16_t gyroTemp;
	 
	 navdata_vec3_t accel;
         navdata_vec3_t gyro;
	 
	 uint32_t alim3V3;
	 uint32_t vrefEpson;
	 uint32_t vrefIDG;
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
	
	void invalidate();
	
	void flatTrim();
	
	void land(const bool emergency = false);
	void takeoff();
	
	void move(const float x, const float y, const float z, const float yaw);
	
	bool requestNavdata();
	bool requestVideo();
	
	void setVideoCodec(const unsigned id);
	
	void configure(const char *const cmd, const char *const value);
	
	void run();
	
	void stop();
	bool isStopped() const;
	
	void control();
	void sendMagic();
	
	void setOwner(const char *const mac);
	void setSsid(const char *const ssid);
	void setNavdataDemo(const bool mode);
	
	bool isControlWatchdog() const;
	bool isLowBatt() const;
	
	void resetControlWatchdog();
	void sendCommWatchdog();
	
	const navdata_t *latestNavdata() const;
	const navdata_demo_t *latestNavdataDemo() const;
	const navdata_raw_t *latestNavdataRawMeasures() const;
	
	bool writeNavdata(const char *const path) const;
	
	void setAtAddress(const Address &atAddress);
	void setNavdataAddress(const Address &atAddress);
	void setVideoAddress(const Address &atAddress);
	
	const Address &atAddress() const;
	const Address &navdataAddress() const;
	const Address &videoAddress() const;
	
	const Private::Image &image() const;
	
private:
	void pushCommand(const char *const command);
	void popCommand();
	void oneTimeCommand(const char *const command);
	bool sendCurrentCommand();
	
	bool fetchNavdata();
	bool fetchVideo();
	
	bool wakeupStream(Socket &socket, const Address &address);
	
	void __enumerateOptions();
	
	bool setupSocket(Socket &socket, const unsigned short bindTo = 0) const;
	
	Socket m_atSocket;
	Socket m_navdataSocket;
	Socket m_videoSocket;
	
	Address m_atAddress;
	Address m_navdataAddress;
	Address m_videoAddress;
	
	Mutex m_mutex;
	Mutex m_control;
	std::stack<DroneCommand> m_commandStack;
	DroneSequencer m_seq;
	bool m_stop;
	unsigned char m_navdata[65507];
	
	const navdata_demo_t *m_navdataDemo;
	const navdata_raw_t *m_navdataRawMeasures;
	
	Private::Image m_image;
};

DroneController::DroneController()
{
	memset(m_navdata, 0, sizeof(m_navdata));
}

DroneController::~DroneController()
{
}

void DroneController::invalidate()
{
	m_mutex.lock();
	m_atSocket.close();
	m_navdataSocket.close();
	m_videoSocket.close();
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
	if(!m_navdataSocket.isOpen()) return true;
	return wakeupStream(m_navdataSocket, m_navdataAddress);
}

bool DroneController::requestVideo()
{
	if(!m_videoSocket.isOpen()) return true;
	return wakeupStream(m_videoSocket, m_videoAddress);
}

void DroneController::setVideoCodec(const unsigned id)
{
	char idbuf[128];
	sprintf(idbuf, "%d", id);
	configure(ARDRONE_VIDEO_CODEC, idbuf);
}

void DroneController::configure(const char *const cmd, const char *const value)
{
	char command[ARDRONE_MAX_CMD_LENGTH];
	sprintf(command, "%s%%u,\"%s\",\"%s\",\"%s\"\r", ARDRONE_AT_CONFIG_IDS,
		ARDRONE_SESSION_ID, ARDRONE_USER_ID, ARDRONE_APPLICATION_ID);
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
			requestVideo();
			requestNavdata();
		}
		
		m_mutex.lock();
		bool success = true;
		
		// Slower update sub-loop
		if(++ticks % 11 == 0) {
			__enumerateOptions();
			success &= sendCurrentCommand();
			success &= fetchVideo();
			if(isControlWatchdog()) std::cout << "control watchdog!" << std::endl;
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
	sprintf(command, "%s%%u,%u,%u,%u,%u\r", ARDRONE_AT_MISC, 2, 20, 2000, 3000);
	oneTimeCommand(command);
}

void DroneController::setOwner(const char *const mac)
{
	configure(ARDRONE_NETWORK_OWNER_MAC, mac);
}

void DroneController::setSsid(const char *const ssid)
{
	configure(ARDRONE_NETWORK_SSID, ssid);
}

void DroneController::setNavdataDemo(const bool mode)
{
	configure(ARDRONE_NAVDATA_DEMO, mode ? ARDRONE_TRUE : ARDRONE_FALSE);
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

void DroneController::setAtAddress(const Address &atAddress)
{
	m_mutex.lock();
	m_atAddress = atAddress;
	
	setupSocket(m_atSocket);
	m_mutex.unlock();
}

void DroneController::setNavdataAddress(const Address &navdataAddress)
{
	m_mutex.lock();
	m_navdataAddress = navdataAddress;
	
	setupSocket(m_navdataSocket, m_navdataAddress.port());
	m_mutex.unlock();
}

void DroneController::setVideoAddress(const Address &videoAddress)
{
	m_mutex.lock();
	m_videoAddress = videoAddress;
	
	setupSocket(m_videoSocket, m_videoAddress.port());
	m_mutex.unlock();
}

const Address &DroneController::atAddress() const
{
	return m_atAddress;
}

const Address &DroneController::navdataAddress() const
{
	return m_navdataAddress;
}

const Address &DroneController::videoAddress() const
{
	return m_videoAddress;
}

const Private::Image &DroneController::image() const
{
	return m_image;
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
	if(!m_navdataSocket.isOpen()) return true;
	
	ssize_t readLength = 0;
	char data[sizeof(m_navdata)];
	if((readLength = m_navdataSocket.recv(data, sizeof(data))) < 0 && errno != EAGAIN) {
		perror("recvfrom");
		return false;
	}
	if(readLength < 0) return true;
	std::cout << "Read " << readLength << " bytes from navdata stream" << std::endl;
	memcpy(m_navdata, data, sizeof(m_navdata));
	return true;
}

bool DroneController::fetchVideo()
{
	if(!m_videoSocket.isOpen()) {
#ifdef ARDRONE_DEBUG
		std::cout << "Video socket is not open. fetchVideo will silently fail." << std::endl;
#endif
		return true;
	}
	
	ssize_t readLength = 0;
	unsigned char data[UVLC_MAX_SIZE];
	if((readLength = m_videoSocket.recv(data, sizeof(data))) < 0 && errno != EAGAIN) {
		perror("recvfrom");
		return false;
	}
	if(readLength < 0) {
#ifdef ARDRONE_DEBUG
		std::cout << "Didn't read any data from video stream." << std::endl;
#endif
		return true;
	}
#ifdef ARDRONE_DEBUG
	std::cout << "Read " << readLength << " bytes from video stream" << std::endl;
#endif
	Private::UvlcVideoDecoder().decode(data, readLength, m_image);
	return true;
}

bool DroneController::wakeupStream(Socket &socket, const Address &address)
{
	if(!socket.isOpen()) {
#ifdef ARDRONE_DEBUG
		std::cout << "Failed to wakeup stream using invalid socket." << std::endl;
#endif
		return false;
	}
	
	const static char dummy[4] = { 0x01, 0x00, 0x00, 0x00 };
	if(socket.sendto(dummy, sizeof(dummy), address) != sizeof(dummy)) {
		perror("DroneController::wakeupStream -> sendto");
		return false;
	}
	
	return true;
}

void DroneController::__enumerateOptions()
{
	const navdata_t *const navdata = latestNavdata();
	if(!navdata || navdata->header != ARDRONE_NAVDATA_HEADER) return;
	
	const char *offset = reinterpret_cast<const char *>(navdata->options);
	for(;;) {
		const navdata_option_t *const option = reinterpret_cast<const navdata_option_t *const>(offset);
		if(option->tag == ARDRONE_NAVDATA_CKS_TAG) break;
		offset += option->size;
		
		switch(option->tag) {
		case ARDRONE_NAVDATA_DEMO_TAG:
			m_navdataDemo = reinterpret_cast<const navdata_demo_t *>(option->data);
			break;
		}
		// std::cout << "Got tag: " << (int)option->tag << std::endl;
		// std::cout << "\tsize:  " << (int)option->size << " (navdata_raw_t size: " << sizeof(navdata_raw_t) << ")" << std::endl;
	}
}

bool DroneController::sendCurrentCommand()
{
	// Nothing to do.
	if(m_commandStack.empty() || !m_atSocket.isOpen()) {
#ifdef ARDRONE_DEBUG
		std::cout << "Waiting on valid socket and non-empty command stack" << std::endl;
#endif
		return true;
	}
	
	char realCommand[ARDRONE_MAX_CMD_LENGTH];
	sprintf(realCommand, m_commandStack.top().data, m_seq.next());
	
	std::cout << "Sending " << realCommand << std::endl;
	
	if(m_atSocket.sendto(realCommand, strlen(realCommand), m_atAddress) < 0) {
		perror("DroneController::run -> sendto");
		return false;
	}
	
	return true;
}

bool DroneController::setupSocket(Socket &socket, const unsigned short bindTo) const
{
	socket.close();
	socket = Socket::udp();
	
	bool success = true;
	success &= socket.setBlocking(false);
	success &= socket.setReusable(true);
	if(bindTo) success &= socket.bind(bindTo);
	if(!success) socket.close();
	
	return success;
}

ARDrone::~ARDrone()
{
	disconnect();
}
	
bool ARDrone::connect(const char *const ip)
{
	m_controller->setAtAddress(Address(ip, ARDRONE_AT_PORT));
	m_controller->setNavdataAddress(Address(ip, ARDRONE_NAVDATA_PORT));
	m_controller->setVideoAddress(Address(ip, ARDRONE_VIDEO_PORT));
	
	m_controller->sendMagic();
	m_controller->setNavdataDemo(true);
	m_controller->setVideoCodec(ARDRONE_VIDEO_UVLC_CODEC);
	m_controller->control();
	
	// Set ourself as the owner
	m_controller->setSsid("Braden's Drone");
	// m_controller->setOwner("48:5d:60:a3:af:c4");
	
	return true;
}

void ARDrone::disconnect()
{
	m_controller->land(true);
	msleep(100);
	// while(state() != ARDrone::Landed) msleep(100);
	
	m_controller->invalidate();
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

cv::Mat ARDrone::video() const
{
	const Private::Image &image = m_controller->image();
	if(image.width <= 0 || image.height <= 0) return cv::Mat();
	cv::Mat mat(image.height, image.width, CV_8UC3);
	memcpy(mat.ptr(), image.data, image.height * image.width * 3);
	cv::Mat bgr = mat;
	cv::cvtColor(bgr, mat, CV_RGB2BGR);
	cv::imshow("AR.Drone Camera", mat);
	return mat;
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
{
	m_controller->start();
}