#ifndef _ACCEL_HPP_
#define _ACCEL_HPP_

#include "export.h"
#include "sensor.hpp"

class Acceleration
{
public:
	static short x();
	static short y();
	static short z();
	static bool calibrate();
private:
	static void setupI2C();
	static bool s_setup;
};

class AccelX : public Sensor<short>
{
public:
	virtual short value() const;
};

class AccelY : public Sensor<short>
{
public:
	virtual short value() const;
};

class AccelZ : public Sensor<short>
{
public:
	virtual short value() const;
};

#endif
