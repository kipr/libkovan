#include "kovan/accel.hpp"
#include "i2c_p.hpp"

#include <unistd.h>
#include <string.h>

#include <stdio.h> //FIXME: remove, only used to debug calib

#define R_XOUT8 0x6
#define R_YOUT8 0x7
#define R_ZOUT8 0x8

#define I2C_CONFIG_ADDY 	0x16
#define SENSITIVITY_2G 		0x4
#define MEASUREMENT_MODE 	0x1

#define R_XBIAS 0x10
#define R_YBIAS 0x12
#define R_ZBIAS 0x14

bool Acceleration::s_setup = false;

signed short Acceleration::x()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	return 4 * (signed short)(signed char)Private::I2C::instance()->read(R_XOUT8);
}

signed short Acceleration::y()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	return 4 * (signed short)(signed char)Private::I2C::instance()->read(R_YOUT8);
}

signed short Acceleration::z()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	return 4 * (signed short)(signed char)Private::I2C::instance()->read(R_ZOUT8);
}

void Acceleration::setupI2C()
{
	if(s_setup) return;
	s_setup = Private::I2C::instance()->pickSlave("0x1d");
	Private::I2C::instance()->write(I2C_CONFIG_ADDY, SENSITIVITY_2G | MEASUREMENT_MODE, false);
}

bool Acceleration::calibrate()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;

	// set biases to zero
	for(int i = 0x10; i < 0x16; i++) {
		Private::I2C::instance()->write(i, 0x0, false);
	}

	usleep(5000);

	// read accel vals
	signed char accel_bias_x = 0;
	signed char accel_bias_y = 0;
	signed char accel_bias_z = 0;

	for(int i = 0; i < 100; i++) {
		signed char accel_x = (signed char)Private::I2C::instance()->read(R_XOUT8);
		signed char accel_y = (signed char)Private::I2C::instance()->read(R_YOUT8);
		signed char accel_z = (signed char)Private::I2C::instance()->read(R_ZOUT8);


		signed short err_sqrd = (accel_x * accel_x)
				+ (accel_y * accel_y)
				+ (accel_z - 64) * (accel_z - 64);

		if(err_sqrd < 17) return true; // success

		// "char" on our platform is unsigned char by default
		// any time a "unsigned" value is used in calculation on ARM
		// it sets the result type to unsigned  so  unsigned = signed + unsigned.
		// for this combination of reasons, it is required to not only cast 2 to (char) but
		// on ARM it also has to be specified as "signed char"
		accel_bias_x += accel_x * (signed char)2;
		accel_bias_y += accel_y * (signed char)2;
		accel_bias_z += (accel_z - 64) * (signed char)2;

		Private::I2C::instance()->write(R_XBIAS, -accel_bias_x, false);
		Private::I2C::instance()->write(R_YBIAS, -accel_bias_y, false);
		Private::I2C::instance()->write(R_ZBIAS, -accel_bias_z, false);

		usleep(5000);
	}

	return false; // fail
}

short AccelX::value() const
{
	return Acceleration::x();
}

short AccelY::value() const
{
	return Acceleration::y();
}

short AccelZ::value() const
{
	return Acceleration::z();
}