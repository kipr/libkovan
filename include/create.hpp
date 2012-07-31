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

	short radius;
	short rightVelocity;
	short leftVelocity;
};

struct CreateSensors
{
	unsigned char bumpsAndWheelDrops;
	unsigned char wall;
	unsigned char cliffLeft;
	unsigned char cliffFrontLeft;
	unsigned char cliffFrontRight;
	unsigned char cliffRight;
	// unsigned char virtualWall;
	unsigned char cargoBayDigitalInputs;
	// unsigned char lowSideDriverAndWheelOvercurrents;
	unsigned char buttons;

	int distance;
	int angle;
	timeval timestamp;
};

struct CreateCalibration
{
	unsigned short refreshRate;
	float angleMultiplier;
};

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

	void setLeds(const bool& advance, const bool& play, const unsigned char& color, const unsigned char& brightness);

	void drive(const short& velocity, const short& radius);
	void driveDirect(const short& left, const short& right);

	void turn(const short& angle, const unsigned short& speed);
	void move(const short& millimeters, const unsigned short& speed);

	void spin(const short& speed);

	short angularVelocity();
	
	inline void spinClockwise(const short& speed) { spin(-speed); }
	inline void spinCounterClockwise(const short& speed) { spin(speed); }
	
	bool setBaudRate(const unsigned char& baudCode);
	
	void updateSensors();
	const CreateSensors *sensors();

	AbstractButton *playButton() const;
	AbstractButton *advanceButton() const;

	Sensor<bool> *wall() const;
	Sensor<bool> *cliffLeft() const;
	Sensor<bool> *cliffFrontLeft() const;
	Sensor<bool> *cliffFrontRight() const;
	Sensor<bool> *cliffRight() const;

	Sensor<bool> *bumpLeft() const;
	Sensor<bool> *bumpRight() const;

	Sensor<bool> *wheelDropLeft() const;
	Sensor<bool> *wheelDropRight() const;
	Sensor<bool> *wheelDropCaster() const;

	void setRefreshRate(const unsigned short& refreshRate);
	const unsigned short& refreshRate() const;

	static Create *instance();
	
private:
	Create();
	Create(const Create& rhs);
	Create& operator=(const Create& rhs);
	
	void setLocalBaudRate(const speed_t& baudRate);
	
	bool start();
	
	bool open();
	void close();

	inline void beginAtomicOperation()
	{
		pthread_mutex_lock(&m_mutex);
	}

	inline void endAtomicOperation()
	{
		pthread_mutex_unlock(&m_mutex);
	}

	inline timeval timeOfDay()
	{
		timeval ret;
		gettimeofday(&ret, NULL);
		return ret;
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
	
	CreateState m_state;
	mutable CreateSensors m_sensors;

	AbstractButton *m_playButton;
	AbstractButton *m_advanceButton;

	Sensor<bool> *m_wall;
	Sensor<bool> *m_cliffLeft;
	Sensor<bool> *m_cliffFrontLeft;
	Sensor<bool> *m_cliffFrontRight;
	Sensor<bool> *m_cliffRight;

	Sensor<bool> *m_bumpLeft;
	Sensor<bool> *m_bumpRight;

	Sensor<bool> *m_wheelDropLeft;
	Sensor<bool> *m_wheelDropRight;
	Sensor<bool> *m_wheelDropCaster;

	unsigned short m_refreshRate;

	size_t m_i;
	CreateScript m_script;
	int m_tty;

	pthread_mutex_t m_mutex;
};

#endif
