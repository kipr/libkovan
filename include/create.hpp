#ifndef _CREATE_HPP_
#define _CREATE_HPP_

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <termios.h>
#include <sys/time.h>
#include <pthread.h>
#include "sensor.hpp"
#include "button.hpp"

#define PI 3.14159f

class CreateScript
{
public:
	CreateScript();
	CreateScript(const CreateScript& rhs);
	
	void append(const unsigned char& byte);
	void append(const unsigned char *data, const size_t& size);
	void append(const CreateScript& script);
	
	void remove(const size_t& i, const size_t& len = 1);
	
	const unsigned char *data() const;
	size_t size() const;
	
	unsigned char byte(const size_t& i);
	
	CreateScript& operator=(const CreateScript& rhs);
	
private:
	std::vector<unsigned char> m_script;
};

struct CreateState
{
	timeval timestamp;

	int distance;
	int angle;
	short radius;
	short rightVelocity;
	short leftVelocity;
};

namespace CreatePackets
{
	struct _1
	{
		unsigned char bumpsAndWheelDrops;
		unsigned char wall;
		unsigned char cliffLeft;
		unsigned char cliffFrontLeft;
		unsigned char cliffFrontRight;
		unsigned char cliffRight;
		unsigned char virtualWall;
		unsigned char cargoBayDigitalInputs;
		unsigned char lowSideDriverAndWheelOvercurrents;
	};

	struct _2
	{
		unsigned char ir;
		unsigned char buttons;
		unsigned char distance[2];
		unsigned char angle[2];
	};

	struct _3
	{
		unsigned char chargingState;
		unsigned char voltage[2];
		char current[2];
		char batteryTemperature;
		unsigned char batteryCharge[2];
		unsigned char batteryCapacity[2];
	};

	struct _4
	{
		unsigned char wallSignal[2];
		unsigned char cliffLeftSignal[2];
		unsigned char cliffFrontLeftSignal[2];
		unsigned char cliffFrontRightSignal[2];
		unsigned char cliffRightSignal[2];
		unsigned char userDigitalInputs;
		unsigned char userAnalogInput[2];
		unsigned char chargingSourcesAvailable;
	};
	
	struct _5
	{
		unsigned char mode;
		unsigned char songNumber;
		unsigned char songPlaying;
		unsigned char numberOfStreamPackets;
		char velocity[2];
		char radius[2];
		char rightVelocity[2];
		char leftVelocity[2];
	};
}

namespace CreateSensors
{
	class PlayButton;
	class AdvanceButton;
	class Wall;
	class CliffLeft;
	class CliffFrontLeft;
	class CliffFrontRight;
	class CliffRight;
	class VirtualWall;
	class WallSignal;
	class CliffLeftSignal;
	class CliffFrontLeftSignal;
	class CliffFrontRightSignal;
	class CliffRightSignal;
	class CargoBayAnalogSignal;
	class CargoBayDigitalInputs;
	class IR;
	class ChargingState;
	class BatteryTemperature;
	class BatteryCharge;
	class BatteryCapacity;
	class Angle;
	class Distance;
	class BumpLeft;
	class BumpRight;
	class WheelDropRight;
	class WheelDropLeft;
	class WheelDropCaster;
}

class Create
{
public:
	enum Mode {
		OffMode,
		PassiveMode,
		SafeMode,
		FullMode
	};
	
	~Create();
	
	bool connect();
	bool disconnect();
	bool isConnected();
	
	void setPassiveMode();
	void setSafeMode();
	void setFullMode();
	
	void setMode(const Create::Mode& mode);
	Create::Mode mode();
	
	void send(const CreateScript& script);
	
	bool write(const unsigned char& c);
	bool write(const unsigned char *data, const size_t& len);
	
	short read();
	int read(unsigned char *data, const size_t& len);
	bool blockingRead(unsigned char *data, const size_t& size);
	
	template<typename T>
	inline bool blockingRead(T& data)
	{
		return blockingRead(reinterpret_cast<unsigned char *>(&data), sizeof(T));
	}

	void setLeds(const bool& advance, const bool& play, const unsigned char& color, const unsigned char& brightness);

	void drive(const short& velocity, const short& radius);
	void driveDirect(const short& left, const short& right);
	inline void driveStraight(const short& speed) { driveDirect(speed, speed); }
	inline void stop() { driveStraight(0); }

	void turn(const short& angle, const unsigned short& speed);
	void move(const short& millimeters, const unsigned short& speed);

	void spin(const short& speed);

	short angularVelocity();
	
	inline void spinClockwise(const short& speed) { spin(-speed); }
	inline void spinCounterClockwise(const short& speed) { spin(speed); }
	
	bool setBaudRate(const unsigned char& baudCode);
	
	// High level sensors
	AbstractButton *playButton();
	AbstractButton *advanceButton();

	Sensor<bool> *wall();
	Sensor<bool> *cliffLeft();
	Sensor<bool> *cliffFrontLeft();
	Sensor<bool> *cliffFrontRight();
	Sensor<bool> *cliffRight();
	Sensor<bool> *virtualWall();
	
	Sensor<unsigned short> *wallSignal();
	Sensor<unsigned short> *cliffLeftSignal();
	Sensor<unsigned short> *cliffFrontLeftSignal();
	Sensor<unsigned short> *cliffFrontRightSignal();
	Sensor<unsigned short> *cliffRightSignal();
	Sensor<unsigned short> *cargoBayAnalogSignal();
	Sensor<unsigned char> *cargoBayDigitalInputs();
	
	Sensor<unsigned char> *ir();
	
	Sensor<unsigned char> *chargingState();
	Sensor<char> *batteryTemperature();
	Sensor<unsigned short> *batteryCharge();
	Sensor<unsigned short> *batteryCapacity();
	
	Sensor<int> *angle();
	Sensor<int> *distance();
	
	Sensor<bool> *bumpLeft();
	Sensor<bool> *bumpRight();

	Sensor<bool> *wheelDropLeft();
	Sensor<bool> *wheelDropRight();
	Sensor<bool> *wheelDropCaster();

	void setRefreshRate(const unsigned short& refreshRate);
	const unsigned short& refreshRate() const;

	static Create *instance();
	
	const CreateState *state();
	const CreatePackets::_1 *sensorPacket1();
	const CreatePackets::_2 *sensorPacket2();
	const CreatePackets::_3 *sensorPacket3();
	const CreatePackets::_4 *sensorPacket4();
	const CreatePackets::_5 *sensorPacket5();
	
	inline void beginAtomicOperation()
	{
		pthread_mutex_lock(&m_mutex);
	}

	inline void endAtomicOperation()
	{
		pthread_mutex_unlock(&m_mutex);
	}
	
private:
	Create();
	Create(const Create& rhs);
	Create& operator=(const Create& rhs);
	
	void setLocalBaudRate(const speed_t& baudRate);
	
	bool start();
	
	bool open();
	void close();
	
	template<typename T>
	inline void lazyDelete(T *&ptr)
	{
		if(ptr) delete ptr;
	}
	
	inline timeval timeOfDay() const
	{
		timeval ret;
		gettimeofday(&ret, NULL);
		return ret;
	}
	
	inline bool hasRequiredTimePassed(const timeval& timestamp) const
	{
		timeval current = timeOfDay();
		timeval result;
		timersub(&current, &timestamp, &result);
		const long msecs = result.tv_sec * 1000 + result.tv_usec / 1000;
		return msecs > m_refreshRate;
	}

	inline double timevalToFloat(const timeval& tv)
	{
		return ((double)tv.tv_sec) + ((double)tv.tv_usec / 1000000.0);
	}

	inline float deg2rad(const float& deg)
	{
		return deg / 180.0 * PI;
	}

	void updateState();
	
	void updateSensorPacket1();
	void updateSensorPacket2();
	void updateSensorPacket3();
	void updateSensorPacket4();
	void updateSensorPacket5();
	
	CreateState m_state;
	
	unsigned short m_refreshRate;
	
	CreatePackets::_1 m_1;
	CreatePackets::_2 m_2;
	CreatePackets::_3 m_3;
	CreatePackets::_4 m_4;
	CreatePackets::_5 m_5;
	timeval timestamps[5];

	CreateSensors::PlayButton *m_playButton;
	CreateSensors::AdvanceButton *m_advanceButton;

	CreateSensors::Wall *m_wall;
	CreateSensors::CliffLeft *m_cliffLeft;
	CreateSensors::CliffFrontLeft *m_cliffFrontLeft;
	CreateSensors::CliffFrontRight *m_cliffFrontRight;
	CreateSensors::CliffRight *m_cliffRight;
	CreateSensors::VirtualWall *m_virtualWall;

	CreateSensors::WallSignal *m_wallSignal;
	CreateSensors::CliffLeftSignal *m_cliffLeftSignal;
	CreateSensors::CliffFrontLeftSignal *m_cliffFrontLeftSignal;
	CreateSensors::CliffFrontRightSignal *m_cliffFrontRightSignal;
	CreateSensors::CliffRightSignal *m_cliffRightSignal;
	CreateSensors::CargoBayAnalogSignal *m_cargoBayAnalogSignal;
	CreateSensors::CargoBayDigitalInputs *m_cargoBayDigitalInputs;
	
	CreateSensors::IR *m_ir;
	
	CreateSensors::ChargingState *m_chargingState;
	CreateSensors::BatteryTemperature *m_batteryTemperature;
	CreateSensors::BatteryCharge *m_batteryCharge;
	CreateSensors::BatteryCapacity *m_batteryCapacity;
	
	CreateSensors::Angle *m_angle;
	CreateSensors::Distance *m_distance;
	
	CreateSensors::BumpLeft *m_bumpLeft;
	CreateSensors::BumpRight *m_bumpRight;

	CreateSensors::WheelDropLeft *m_wheelDropLeft;
	CreateSensors::WheelDropRight *m_wheelDropRight;
	CreateSensors::WheelDropCaster *m_wheelDropCaster;

	size_t m_i;
	CreateScript m_script;
	int m_tty;

	pthread_mutex_t m_mutex;
};

#endif
