#include "create.hpp"
#include "create_codes.h"
#include "util.hpp"

#include <fcntl.h>
#include <unistd.h>
#include <cstring>

#include <cstdio>
#include <cmath>
#include <pthread.h>
#include <iostream>
#include <cstring>

#define LOW_BYTE(x) ((x) & 0xFF)
#define HIGH_BYTE(x) (((x) & 0xFF00) >> 8)

#define SHORT(low, high) ((high << 8) | low)

const static unsigned int baudCodeRate[12] = {
	300, 600, 1200, 2400, 4800, 9600,
	14400, 19200, 28800, 38400, 57600,
	115200
};

////////////////////
// CREATE SENSORS //
////////////////////

class PlayButton : public AbstractButton
{
public:
	PlayButton(Create *create) : m_create(create) {}

	virtual void setPressed(bool pressed) {}
	virtual bool value() const
	{
		return m_create->sensors()->buttons & 0x01;
	}

private:
	Create *m_create;
};

class AdvanceButton : public AbstractButton
{
public:
	AdvanceButton(Create *create) : m_create(create) {}

	virtual void setPressed(bool pressed) {}
	virtual bool value() const
	{
		return m_create->sensors()->buttons & 0x04;
	}

private:
	Create *m_create;
};

class WallSensor : public Sensor<bool>
{
public:
	WallSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->wall;
	}
private:
	Create *m_create;
};

class CliffLeftSensor : public Sensor<bool>
{
public:
	CliffLeftSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->cliffLeft;
	}
private:
	Create *m_create;
};

class CliffFrontLeftSensor : public Sensor<bool>
{
public:
	CliffFrontLeftSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->cliffFrontLeft;
	}
private:
	Create *m_create;
};

class CliffFrontRightSensor : public Sensor<bool>
{
public:
	CliffFrontRightSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->cliffFrontRight;
	}
private:
	Create *m_create;
};

class CliffRightSensor : public Sensor<bool>
{
public:
	CliffRightSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->cliffRight;
	}
private:
	Create *m_create;
};

class BumpLeftSensor : public Sensor<bool>
{
public:
	BumpLeftSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->bumpsAndWheelDrops & 0x02;
	}
private:
	Create *m_create;
};

class BumpRightSensor : public Sensor<bool>
{
public:
	BumpRightSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->bumpsAndWheelDrops & 0x01;
	}
private:
	Create *m_create;
};

class WheelDropRightSensor : public Sensor<bool>
{
public:
	WheelDropRightSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->bumpsAndWheelDrops & 0x04;
	}
private:
	Create *m_create;
};

class WheelDropLeftSensor : public Sensor<bool>
{
public:
	WheelDropLeftSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->bumpsAndWheelDrops & 0x08;
	}
private:
	Create *m_create;
};

class WheelDropCasterSensor : public Sensor<bool>
{
public:
	WheelDropCasterSensor(Create *create) : m_create(create) {}

	virtual bool value() const
	{
		return m_create->sensors()->bumpsAndWheelDrops & 0x10;
	}
private:
	Create *m_create;
};

////////////////////
// CREATE SCRIPTS //
////////////////////

CreateScript::CreateScript()
{
	
}

CreateScript::CreateScript(const CreateScript& rhs)
{
	append(rhs);
}

void CreateScript::append(const unsigned char& byte)
{
	append(&byte, 1);
}

void CreateScript::append(const unsigned char *data, const size_t& size)
{
	m_script.reserve(m_script.size() + size);
	for(size_t i = 0; i < size; ++i) m_script.push_back(data[i]);
}

void CreateScript::append(const CreateScript& script)
{
	append(script.data(), script.size());
}

void CreateScript::remove(const size_t& i, const size_t& len)
{
	if(len == 0) return;
	std::vector<unsigned char>::iterator it = m_script.begin();
	m_script.erase(it + i, it + i + len);
}

const unsigned char *CreateScript::data() const
{
	return &m_script[0]; // This is pretty jank
}

size_t CreateScript::size() const
{
	return m_script.size();
}

unsigned char CreateScript::byte(const size_t& i)
{
	return m_script[i];
}

CreateScript& CreateScript::operator=(const CreateScript& rhs)
{
	m_script = rhs.m_script;
	return *this;
}

///////////////////
// CREATE DEVICE //
///////////////////

Create::~Create()
{
	disconnect();
	delete m_playButton;
	delete m_advanceButton;

	pthread_mutex_destroy(&m_mutex);
}

int
set_interface_attribs (int fd, int speed, int parity)
{
    struct termios options;   

    fcntl (fd, F_SETFL, 0);
    tcflush (fd, TCIOFLUSH);

    //get config from fd and put into options
    tcgetattr (fd, &options); 
    //give raw data path
    cfmakeraw (&options);
    //set baud
    cfsetispeed (&options, B57600);                 
    cfsetospeed (&options, B57600);
    //send options back to fd
    tcsetattr (fd, TCSANOW, &options);

    return 0;
}

bool Create::connect()
{
	if(!open()) return false;
	if(set_interface_attribs(m_tty, B57600, 0) != 0) {
		close();
		return false;
	}
	
	// setLocalBaudRate(baudCodeRate[10]); // This is the default rate
	start();
	
	// Make the connection baud rate 115200
	// setBaudRate(11);
	
	setMode(SafeMode);
	if(mode() != SafeMode) {
		close();
		return false;
	}
	
	updateSensors();

	return true;
	
}

bool Create::disconnect()
{
	close();
	return true;
}

bool Create::isConnected()
{
	return m_tty;
}

void Create::setPassiveMode()
{
	beginAtomicOperation();
	start();
	endAtomicOperation();
}

void Create::setSafeMode()
{
	beginAtomicOperation();
	write(OI_SAFE);
	endAtomicOperation();
}

void Create::setFullMode()
{
	beginAtomicOperation();
	write(OI_FULL);
	endAtomicOperation();
}

void Create::setMode(const Create::Mode& mode)
{
	switch(mode) {
	case OffMode: // NYI
	case PassiveMode: setPassiveMode(); break;
	case SafeMode: setSafeMode(); break;
	case FullMode: setFullMode(); break;
	}
}

Create::Mode Create::mode()
{
	beginAtomicOperation();
	write(OI_SENSORS);
	write(35);
	short state = 0;
	do {
		state = read();
		if(state < 0) return OffMode;
	} while(state == 0);
	endAtomicOperation();

	printf("State = %d\n", state);
        switch(state) {
	case 0: return OffMode;
	case 1: return PassiveMode;
	case 2: return SafeMode;
	case 3: return FullMode;
	}
	return OffMode;
}

void Create::send(const CreateScript& script)
{
	m_script = script;
}

bool Create::write(const unsigned char& c)
{
	return write(&c, 1);
}

bool Create::write(const unsigned char *data, const size_t& len)
{
	if(!m_tty) return false;
	return ::write(m_tty, data, len) == len;
}

short Create::read()
{
	unsigned char ret = 0;
	return read(&ret, 1) == 1 ? ret : -1;
}

int Create::read(unsigned char *data, const size_t& len)
{
	if(!m_tty) return 0;
	int ret = ::read(m_tty, data, len);
	if(ret < 0) perror("::read");
	return ret;
}

void Create::setLeds(const bool& advance, const bool& play, const unsigned char& color, const unsigned char& brightness)
{
	beginAtomicOperation();
	write(OI_LEDS);
	unsigned char packed = 0;
	if(advance) packed |= 0x4;
	if(play) packed |= 0x2;
	write(packed);
	write(color);
	write(brightness);
	endAtomicOperation();
}

void Create::drive(const short& velocity, const short& radius)
{
	beginAtomicOperation();

	write(OI_DRIVE);
	write(HIGH_BYTE(velocity));
	write(LOW_BYTE(velocity));
	write(HIGH_BYTE(radius));
	write(LOW_BYTE(radius));

	m_state.radius = radius;
	m_state.leftVelocity = -velocity;
	m_state.rightVelocity = velocity;
	updateState();

	endAtomicOperation();
}

void Create::driveDirect(const short& left, const short& right)
{
	beginAtomicOperation();

	write(OI_DRIVE_DIRECT);
	write(HIGH_BYTE(right));
	write(LOW_BYTE(right));
	write(HIGH_BYTE(left));
	write(LOW_BYTE(left));

	m_state.radius = 0xFFFF;
	m_state.leftVelocity = left;
	m_state.rightVelocity = right;
	
	updateState();

	endAtomicOperation();
}

void Create::turn(const short& angle, const unsigned short& speed)
{
	spin(angle > 0 ? speed : -speed);
	const short goalAngle = m_sensors.angle + angle;
	double timeToGoal = (deg2rad(angle + 360 / angle) * 258) / angularVelocity();
	printf("Time to Goal: %f (rad = %f, av = %d)\n", timeToGoal, deg2rad(angle), angularVelocity());
	double startTime = timevalToFloat(timeOfDay());
	usleep(timeToGoal * 1000000L - 300);
	double elapsed = timevalToFloat(timeOfDay());
	printf("Diff: %lf\n", elapsed - startTime - timeToGoal);
	m_sensors.angle = goalAngle;
	spin(0);
}

void Create::move(const short& millimeters, const unsigned short& speed)
{
	driveDirect(speed, speed);
	const short goalDistance = m_sensors.distance + millimeters;
	double timeToGoal = ((double)millimeters) / speed;
	printf("Time to Goal: %f (milli = %d, s = %d)\n", timeToGoal, millimeters, speed);
	double startTime = timevalToFloat(timeOfDay());
	usleep(timeToGoal * 1000000L - 300);
	double elapsed = timevalToFloat(timeOfDay());
	printf("Diff: %lf\n", elapsed - startTime - timeToGoal);
	driveDirect(0, 0);
}

void Create::spin(const short& speed)
{
	drive(speed, 1);
	m_state.leftVelocity = -speed;
	m_state.rightVelocity = speed;
	updateState();
}

short Create::angularVelocity()
{
	return m_state.rightVelocity - m_state.leftVelocity;
}

bool Create::setBaudRate(const unsigned char& baudCode)
{
	if(!m_tty || baudCode >= 12) return false;

	beginAtomicOperation();
	
	// Tell the create to update its baud rate
	write(OI_BAUD);
	write(baudCode);
	
	// Update our baud rate
	setLocalBaudRate(baudCodeRate[baudCode]);
	
	endAtomicOperation();

	return true;
}

const CreateSensors *Create::sensors()
{
	updateSensors();
	return &m_sensors;
}

AbstractButton *Create::playButton() const
{
	return m_playButton;
}

AbstractButton *Create::advanceButton() const
{
	return m_advanceButton;
}

Sensor<bool> *Create::wall() const
{
	return m_wall;
}

Sensor<bool> *Create::cliffLeft() const
{
	return m_cliffLeft;
}

Sensor<bool> *Create::cliffFrontLeft() const
{
	return m_cliffFrontLeft;
}

Sensor<bool> *Create::cliffFrontRight() const
{
	return m_cliffFrontRight;
}

Sensor<bool> *Create::cliffRight() const
{
	return m_cliffRight;
}

Sensor<bool> *Create::bumpLeft() const
{
	return m_bumpLeft;
}

Sensor<bool> *Create::bumpRight() const
{
	return m_bumpRight;
}

Sensor<bool> *Create::wheelDropLeft() const
{
	return m_wheelDropLeft;
}

Sensor<bool> *Create::wheelDropRight() const
{
	return m_wheelDropRight;
}

Sensor<bool> *Create::wheelDropCaster() const
{
	return m_wheelDropCaster;
}

void Create::setRefreshRate(const unsigned short& refreshRate)
{
	m_refreshRate = refreshRate;
}

const unsigned short& Create::refreshRate() const
{
	return m_refreshRate;
}

Create *Create::instance()
{
	static Create s_create;
	return &s_create;
}

Create::Create()
	: m_refreshRate(150),
	m_playButton(new PlayButton(this)),
	m_advanceButton(new AdvanceButton(this)),
	m_wall(new WallSensor(this)),
	m_cliffLeft(new CliffLeftSensor(this)),
	m_cliffFrontLeft(new CliffFrontLeftSensor(this)),
	m_cliffFrontRight(new CliffFrontRightSensor(this)),
	m_cliffRight(new CliffRightSensor(this)),
	m_bumpLeft(new BumpLeftSensor(this)),
	m_bumpRight(new BumpRightSensor(this)),
	m_wheelDropLeft(new WheelDropLeftSensor(this)),
	m_wheelDropRight(new WheelDropRightSensor(this)),
	m_wheelDropCaster(new WheelDropCasterSensor(this))
{
	pthread_mutex_init(&m_mutex, 0);
}

Create::Create(const Create&) {}
Create& Create::operator=(const Create&) { return *this; }

void Create::setLocalBaudRate(const speed_t& baudRate)
{
	struct termios attribs;
	tcgetattr(m_tty, &attribs);
	
	cfsetispeed(&attribs, baudRate);
	cfsetospeed(&attribs, baudRate);
	
	tcsetattr(m_tty, TCSADRAIN, &attribs);
}

bool Create::start()
{
	if(!m_tty) return false;

	beginAtomicOperation();
	const bool ret = write(OI_START);
	endAtomicOperation();
	
	return ret;
}

bool Create::open()
{
	if(m_tty) return false;
	
	beginAtomicOperation();
	m_tty = ::open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
	if(m_tty < 0) perror("Create::open");
	endAtomicOperation();

	return m_tty >= 0;
}

void Create::close()
{
	if(!m_tty) return;
	beginAtomicOperation();
	::close(m_tty);
	m_tty = 0;
	endAtomicOperation();
}

void Create::updateState()
{
	m_state.timestamp = timeOfDay();
}

void printArray(const unsigned char *array, const size_t& size) {
	for(size_t i = 0; i < size; ++i) printf("%x ", array[i]);
	printf("\n");
}

struct CreatePacket
{
	unsigned char bumpsAndWheelDrops;
	unsigned char wall;
	unsigned char cliffLeft;
	unsigned char cliffFrontLeft;
	unsigned char cliffFrontRight;
	unsigned char cliffRight;
	// unsigned char virtualWall;
	// unsigned char lowSideDriverAndWheelOvercurrents;
	unsigned char cargoBayDigitalInputs;
	unsigned char buttons;

	unsigned char distanceHigh;
	unsigned char distanceLow;
	unsigned char angleHigh;
	unsigned char angleLow;
};


void Create::updateSensors()
{
	timeval difference = timeOfDay();
	difference.tv_sec -= m_sensors.timestamp.tv_sec;
	if(!difference.tv_sec) {
		difference.tv_usec -= m_sensors.timestamp.tv_usec;
		if(difference.tv_usec / 1000 < m_refreshRate) return;
	}

	beginAtomicOperation();

	write(OI_QUERY_LIST);
	static const unsigned char sensors[] = {
		OI_BUMPS_AND_WHEEL_DROPS,
		OI_WALL,
		OI_CLIFF_LEFT,
		OI_CLIFF_FRONT_LEFT,
		OI_CLIFF_FRONT_RIGHT,
		OI_CLIFF_RIGHT,
		OI_BUTTONS,
		OI_DISTANCE,
		OI_ANGLE
	};
	static const unsigned char size = sizeof(sensors);
	write(size);
	write(sensors, size);

	size_t total = 0;
	static const size_t expectedSize = sizeof(CreatePacket);
	CreatePacket packet;
	while(total < expectedSize) total += read(reinterpret_cast<unsigned char *>(&packet) + total, expectedSize - total);
	
	memcpy(&m_sensors, &packet, sizeof(CreatePacket) - sizeof(short) * 2);
	m_sensors.angle += SHORT(packet.angleLow, packet.angleHigh);
	m_sensors.distance += SHORT(packet.distanceLow, packet.distanceHigh);

	printArray(reinterpret_cast<unsigned char *>(&packet), expectedSize);
	m_sensors.timestamp = timeOfDay();

	endAtomicOperation();
}