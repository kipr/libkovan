#include "kovan/accel.hpp"
#include "i2c_p.hpp"
#include <unistd.h>

#define R_XOUT8 0x6
#define R_YOUT8 0x7
#define R_ZOUT8 0x8

#define I2C_CONFIG_ADDY		0x16
#define SENSITIVITY_2G		0x4
#define MEASUREMENT_MODE	0x1


#define R_XBIAS 0x10
#define R_YBIAS 0x12
#define R_ZBIAS 0x14

bool Acceleration::s_setup = false;

short Acceleration::x()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	short x_accel = Private::I2C::instance()->read(R_XOUT8);
	if (x_accel > 127) x_accel = 127 - x_accel;
	return (4 * x_accel);
}

short Acceleration::y()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	short y_accel = Private::I2C::instance()->read(R_YOUT8);
	if (y_accel > 127) y_accel = 127 - y_accel;
	return (4 * y_accel);
}

short Acceleration::z()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	short z_accel = Private::I2C::instance()->read(R_ZOUT8);
	if (z_accel > 127) z_accel = 127 - z_accel;
	return (4 * z_accel);
}

void Acceleration::setupI2C()
{
	if(s_setup) return;
	s_setup = Private::I2C::instance()->pickSlave("0x1d");
	Private::I2C::instance()->write(I2C_CONFIG_ADDY, (SENSITIVITY_2G | MEASUREMENT_MODE), false);
}

bool Acceleration::calibrate()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;

	// set biases to zero
	for (int i = 0x10; i < 0x16; i++){
		Private::I2C::instance()->write(i, 0x0, false);
	}

	usleep(5000);

	// read accel vals
	short accel_bias_x = 0;
	short accel_bias_y = 0;
	short accel_bias_z = 0;

	short accel_x = 0;
	short accel_y = 0;
	short accel_z = 0;

	for (int i = 0; i < 100; i++){

		accel_x = (short)(char)Private::I2C::instance()->read(R_XOUT8);
        accel_y = (short)(char)Private::I2C::instance()->read(R_YOUT8);
       	accel_z = (short)(char)Private::I2C::instance()->read(R_ZOUT8);

		if (((accel_x * accel_x) + (accel_y * accel_y) + (accel_z - 64)*(accel_z-64)) < 17){
			return true; // success
		}

   		accel_bias_x += accel_x * 2;
  		accel_bias_y += accel_y * 2;
      	accel_bias_z += (accel_z-64) * 2;

      	Private::I2C::instance()->write(R_XBIAS, -accel_bias_x , false);
      	Private::I2C::instance()->write(R_YBIAS, -accel_bias_y, false);
      	Private::I2C::instance()->write(R_ZBIAS, -accel_bias_z, false);
	}

	return false; // fail
}
