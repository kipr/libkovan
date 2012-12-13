#include "kovan/accel.hpp"
#include "i2c_p.hpp"

#define DATA_ADDR 0x16
#define DATA_VAL 0x05

#define READ_OFFSET 6

bool Acceleration::s_setup = false;

short Acceleration::x()
{
	setupI2C();
	if(s_setup) return 0xFFFF;
	return static_cast<char>(Private::I2C::instance()->read(READ_OFFSET + 0));
}

short Acceleration::y()
{
	setupI2C();
	if(s_setup) return 0xFFFF;
	return static_cast<char>(Private::I2C::instance()->read(READ_OFFSET + 1));
}

short Acceleration::z()
{
	setupI2C();
	if(s_setup) return 0xFFFF;
	return static_cast<char>(Private::I2C::instance()->read(READ_OFFSET + 2));
}

void Acceleration::setupI2C()
{
	if(s_setup) return;
	s_setup = Private::I2C::instance()->pickSlave("0x1d");
	Private::I2C::instance()->write(DATA_ADDR, DATA_VAL, true);
}