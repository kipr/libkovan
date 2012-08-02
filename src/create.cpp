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

#define SHORT_FROM_BYTES(high, low) (((high) << 8) | (low))
#define SHORT(array) SHORT_FROM_BYTES(array[0], array[1])

template<typename T>
T *construct(T *dummy, const Create *parent)
{
	return new T(const_cast<Create *>(parent));
}

#define LAZY_RETURN(x) \
{ \
	if(!x) x = construct(x, this); \
	return x; \
}

const static unsigned int baudCodeRate[12] = {
	300, 600, 1200, 2400, 4800, 9600,
	14400, 19200, 28800, 38400, 57600,
	115200
};

using namespace CreatePackets;

////////////////////
// CREATE SENSORS //
////////////////////

namespace CreateSensors
{
	class PlayButton : public AbstractButton
	{
	public:
		PlayButton(Create *create) : m_create(create) {}
		
		virtual void setPressed(bool pressed) {}
		virtual bool value() const
		{
			return m_create->sensorPacket2()->buttons & 0x01;
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
			return m_create->sensorPacket2()->buttons & 0x04;
		}

	private:
		Create *m_create;
	};

	class Wall : public Sensor<bool>
	{
	public:
		Wall(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->wall;
		}
	private:
		Create *m_create;
	};

	class CliffLeft : public Sensor<bool>
	{
	public:
		CliffLeft(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->cliffLeft;
		}
	private:
		Create *m_create;
	};

	class CliffFrontLeft : public Sensor<bool>
	{
	public:
		CliffFrontLeft(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->cliffFrontLeft;
		}
	private:
		Create *m_create;
	};

	class CliffFrontRight : public Sensor<bool>
	{
	public:
		CliffFrontRight(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->cliffFrontRight;
		}
	private:
		Create *m_create;
	};

	class CliffRight : public Sensor<bool>
	{
	public:
		CliffRight(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->cliffRight;
		}
	private:
		Create *m_create;
	};



	class VirtualWall : public Sensor<bool>
	{
	public:
		VirtualWall(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->virtualWall;
		}
	private:
		Create *m_create;
	};

	class WallSignal : public Sensor<unsigned short>
	{
	public:
		WallSignal(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket4()->wallSignal);
		}
	private:
		Create *m_create;
	};

	class CliffLeftSignal : public Sensor<unsigned short>
	{
	public:
		CliffLeftSignal(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket4()->cliffLeftSignal);
		}
	private:
		Create *m_create;
	};

	class CliffFrontLeftSignal : public Sensor<unsigned short>
	{
	public:
		CliffFrontLeftSignal(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket4()->cliffFrontLeftSignal);
		}
	private:
		Create *m_create;
	};

	class CliffFrontRightSignal : public Sensor<unsigned short>
	{
	public:
		CliffFrontRightSignal(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket4()->cliffFrontRightSignal);
		}
	private:
		Create *m_create;
	};

	class CliffRightSignal : public Sensor<unsigned short>
	{
	public:
		CliffRightSignal(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket4()->cliffRightSignal);
		}
	private:
		Create *m_create;
	};

	class CargoBayAnalogSignal : public Sensor<unsigned short>
	{
	public:
		CargoBayAnalogSignal(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket4()->userAnalogInput);
		}
	private:
		Create *m_create;
	};
	
	class CargoBayDigitalInputs : public Sensor<unsigned char>
	{
	public:
		CargoBayDigitalInputs(Create *create) : m_create(create) {}

		virtual unsigned char value() const
		{
			return m_create->sensorPacket4()->userDigitalInputs;
		}
	private:
		Create *m_create;
	};

	class IR : public Sensor<unsigned char>
	{
	public:
		IR(Create *create) : m_create(create) {}

		virtual unsigned char value() const
		{
			return m_create->sensorPacket2()->ir;
		}
	private:
		Create *m_create;
	};

	class ChargingState : public Sensor<unsigned char>
	{
	public:
		ChargingState(Create *create) : m_create(create) {}

		virtual unsigned char value() const
		{
			return m_create->sensorPacket3()->chargingState;
		}
	private:
		Create *m_create;
	};

	class BatteryTemperature : public Sensor<char>
	{
	public:
		BatteryTemperature(Create *create) : m_create(create) {}

		virtual char value() const
		{
			return m_create->sensorPacket3()->batteryTemperature;
		}
	private:
		Create *m_create;
	};

	class BatteryCharge : public Sensor<unsigned short>
	{
	public:
		BatteryCharge(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket3()->batteryCharge);
		}
	private:
		Create *m_create;
	};

	class BatteryCapacity : public Sensor<unsigned short>
	{
	public:
		BatteryCapacity(Create *create) : m_create(create) {}

		virtual unsigned short value() const
		{
			return SHORT(m_create->sensorPacket3()->batteryCapacity);
		}
	private:
		Create *m_create;
	};

	class Angle : public Sensor<int>
	{
	public:
		Angle(Create *create) : m_create(create) {}

		virtual int value() const
		{
			return m_create->state()->angle;
		}
	private:
		Create *m_create;
	};

	class Distance : public Sensor<int>
	{
	public:
		Distance(Create *create) : m_create(create) {}

		virtual int value() const
		{
			return m_create->state()->distance;
		}
	private:
		Create *m_create;
	};

	class BumpLeft : public Sensor<bool>
	{
	public:
		BumpLeft(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->bumpsAndWheelDrops & 0x02;
		}
	private:
		Create *m_create;
	};

	class BumpRight : public Sensor<bool>
	{
	public:
		BumpRight(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->bumpsAndWheelDrops & 0x01;
		}
	private:
		Create *m_create;
	};

	class WheelDropRight : public Sensor<bool>
	{
	public:
		WheelDropRight(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->bumpsAndWheelDrops & 0x04;
		}
	private:
		Create *m_create;
	};

	class WheelDropLeft : public Sensor<bool>
	{
	public:
		WheelDropLeft(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->bumpsAndWheelDrops & 0x08;
		}
	private:
		Create *m_create;
	};

	class WheelDropCaster : public Sensor<bool>
	{
	public:
		WheelDropCaster(Create *create) : m_create(create) {}

		virtual bool value() const
		{
			return m_create->sensorPacket1()->bumpsAndWheelDrops & 0x10;
		}
	private:
		Create *m_create;
	};
}

using namespace CreateSensors;

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

	lazyDelete(m_playButton);
	lazyDelete(m_advanceButton);
	lazyDelete(m_wall);
	lazyDelete(m_cliffLeft);
	lazyDelete(m_cliffFrontLeft);
	lazyDelete(m_cliffFrontRight);
	lazyDelete(m_cliffRight);
	lazyDelete(m_virtualWall);
	lazyDelete(m_wallSignal);
	lazyDelete(m_cliffLeftSignal);
	lazyDelete(m_cliffFrontLeftSignal);
	lazyDelete(m_cliffFrontRightSignal);
	lazyDelete(m_cliffRightSignal);
	lazyDelete(m_cargoBayAnalogSignal);
	lazyDelete(m_ir);
	lazyDelete(m_chargingState);
	lazyDelete(m_batteryTemperature);
	lazyDelete(m_batteryCharge);
	lazyDelete(m_batteryCapacity);
	lazyDelete(m_angle);
	lazyDelete(m_distance);
	lazyDelete(m_bumpLeft);
	lazyDelete(m_bumpRight);
	lazyDelete(m_wheelDropLeft);
	lazyDelete(m_wheelDropRight);
	lazyDelete(m_wheelDropCaster);

	pthread_mutex_destroy(&m_mutex);
}


// TODO: Clean this up
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

bool Create::blockingRead(unsigned char *data, const size_t& size)
{
	size_t total = 0;
	while(total < size) {
		int ret = read(data + total, size - total);

		if(ret < 0) return false;
		total += ret;
	}
	return true;
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
	const short goalAngle = m_state.angle + angle;
	double timeToGoal = (deg2rad(angle + 360 / angle) * 258) / angularVelocity();
	printf("Time to Goal: %f (rad = %f, av = %d)\n", timeToGoal, deg2rad(angle), angularVelocity());
	double startTime = timevalToFloat(timeOfDay());
	usleep(timeToGoal * 1000000L - 300);
	double elapsed = timevalToFloat(timeOfDay());
	printf("Diff: %lf\n", elapsed - startTime - timeToGoal);
	spin(0);
}

void Create::move(const short& millimeters, const unsigned short& speed)
{
	driveDirect(speed, speed);
	const short goalDistance = m_state.distance + millimeters;
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

const CreateState *Create::state()
{
	return &m_state;
}

const CreatePackets::_1 *Create::sensorPacket1()
{
	updateSensorPacket1();
	return &m_1;
}

const CreatePackets::_2 *Create::sensorPacket2()
{
	updateSensorPacket2();
	return &m_2;
}

const CreatePackets::_3 *Create::sensorPacket3()
{
	updateSensorPacket3();
	return &m_3;
}

const CreatePackets::_4 *Create::sensorPacket4()
{
	updateSensorPacket4();
	return &m_4;
}

const CreatePackets::_5 *Create::sensorPacket5()
{
	updateSensorPacket5();
	return &m_5;
}

AbstractButton *Create::playButton() LAZY_RETURN(m_playButton);
AbstractButton *Create::advanceButton() LAZY_RETURN(m_advanceButton);
Sensor<bool> *Create::wall() LAZY_RETURN(m_wall);
Sensor<bool> *Create::cliffLeft() LAZY_RETURN(m_cliffLeft);
Sensor<bool> *Create::cliffFrontLeft() LAZY_RETURN(m_cliffFrontLeft);
Sensor<bool> *Create::cliffFrontRight() LAZY_RETURN(m_cliffFrontRight);
Sensor<bool> *Create::cliffRight() LAZY_RETURN(m_cliffRight);
Sensor<bool> *Create::virtualWall() LAZY_RETURN(m_virtualWall);
Sensor<unsigned short> *Create::wallSignal() LAZY_RETURN(m_wallSignal);
Sensor<unsigned short> *Create::cliffLeftSignal() LAZY_RETURN(m_cliffLeftSignal);
Sensor<unsigned short> *Create::cliffFrontLeftSignal() LAZY_RETURN(m_cliffFrontLeftSignal);
Sensor<unsigned short> *Create::cliffFrontRightSignal() LAZY_RETURN(m_cliffFrontRightSignal);
Sensor<unsigned short> *Create::cliffRightSignal() LAZY_RETURN(m_cliffRightSignal);
Sensor<unsigned short> *Create::cargoBayAnalogSignal() LAZY_RETURN(m_cargoBayAnalogSignal);
Sensor<unsigned char> *Create::cargoBayDigitalInputs() LAZY_RETURN(m_cargoBayDigitalInputs);
Sensor<unsigned char> *Create::ir() LAZY_RETURN(m_ir);
Sensor<unsigned char> *Create::chargingState() LAZY_RETURN(m_chargingState);
Sensor<char> *Create::batteryTemperature() LAZY_RETURN(m_batteryTemperature);
Sensor<unsigned short> *Create::batteryCharge() LAZY_RETURN(m_batteryCharge);
Sensor<unsigned short> *Create::batteryCapacity() LAZY_RETURN(m_batteryCapacity);
Sensor<int> *Create::angle() LAZY_RETURN(m_angle);
Sensor<int> *Create::distance() LAZY_RETURN(m_distance);
Sensor<bool> *Create::bumpLeft() LAZY_RETURN(m_bumpLeft);
Sensor<bool> *Create::bumpRight() LAZY_RETURN(m_bumpRight);
Sensor<bool> *Create::wheelDropLeft() LAZY_RETURN(m_wheelDropLeft);
Sensor<bool> *Create::wheelDropRight() LAZY_RETURN(m_wheelDropRight);
Sensor<bool> *Create::wheelDropCaster() LAZY_RETURN(m_wheelDropCaster);

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
	: m_refreshRate(10),
	m_playButton(0),
	m_advanceButton(0),
	m_wall(0),
	m_cliffLeft(0),
	m_cliffFrontLeft(0),
	m_cliffFrontRight(0),
	m_cliffRight(0),
	m_virtualWall(0),
	m_wallSignal(0),
	m_cliffLeftSignal(0),
	m_cliffFrontLeftSignal(0),
	m_cliffFrontRightSignal(0),
	m_cliffRightSignal(0),
	m_cargoBayAnalogSignal(0),
	m_cargoBayDigitalInputs(0),
	m_ir(0),
	m_chargingState(0),
	m_batteryTemperature(0),
	m_batteryCharge(0),
	m_batteryCapacity(0),
	m_angle(0),
	m_distance(0),
	m_bumpLeft(0),
	m_bumpRight(0),
	m_wheelDropLeft(0),
	m_wheelDropRight(0),
	m_wheelDropCaster(0)
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
	m_tty = ::open("/dev/ttyS2", O_RDWR | O_NOCTTY | O_NDELAY | O_NONBLOCK);
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

template<typename T>
void printArray(const T& array) {
	printArray(reinterpret_cast<const unsigned char *>(&array), sizeof(T));
}

void Create::updateSensorPacket1()
{
	if(!hasRequiredTimePassed(timestamps[0])) return;
	
	beginAtomicOperation();
	write(OI_SENSORS);
	write(1);
	blockingRead(m_1);
	timestamps[0] = timeOfDay();
	endAtomicOperation();
}

void Create::updateSensorPacket2()
{
	if(!hasRequiredTimePassed(timestamps[1])) return;
	beginAtomicOperation();
	write(OI_SENSORS);
	write(2);
	blockingRead(m_2);
	printArray(m_2);
	m_state.distance += SHORT(m_2.distance);
	m_state.angle += SHORT(m_2.angle);
	timestamps[1] = timeOfDay();
	endAtomicOperation();
}

void Create::updateSensorPacket3()
{
	if(!hasRequiredTimePassed(timestamps[2])) return;
	
	beginAtomicOperation();
	write(OI_SENSORS);
	write(3);
	blockingRead(m_3);
	timestamps[2] = timeOfDay();
	endAtomicOperation();
}

void Create::updateSensorPacket4()
{
	if(!hasRequiredTimePassed(timestamps[3])) return;
	
	beginAtomicOperation();
	write(OI_SENSORS);
	write(4);
	blockingRead(m_4);
	timestamps[3] = timeOfDay();
	endAtomicOperation();
}

void Create::updateSensorPacket5()
{
	if(!hasRequiredTimePassed(timestamps[4])) return;
	
	beginAtomicOperation();
	write(OI_SENSORS);
	write(5);
	blockingRead(m_5);
	timestamps[4] = timeOfDay();
	endAtomicOperation();
}