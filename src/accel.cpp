#include "kovan/accel.hpp"
#include "i2c_p.hpp"

#define DATA_ADDR 16
#define DATA_VAL 0x05

#define R_XOUTL 0x0
#define R_XOUTH 0x1

#define R_YOUTL 0x2
#define R_YOUTH 0x3

#define R_ZOUTL 0x4
#define R_ZOUTH 0x5

bool Acceleration::s_setup = false;

short Acceleration::x()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	// it is important that low is read before high
	// reading the low 8 bits latches the high 2 bits (page 21)
	// high bits must be read immediately afterwards
	short x_low = static_cast<short>(Private::I2C::instance()->read(R_XOUTL));
	short x_high = static_cast<short>(Private::I2C::instance()->read(R_XOUTH));
	if(x_high & 0x02) x_high |= 0xFE;
	return ((x_high << 8) | x_low);
}

short Acceleration::y()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	short y_low = static_cast<short>(Private::I2C::instance()->read(R_YOUTL));
	short y_high = static_cast<short>(Private::I2C::instance()->read(R_YOUTH));
	if(y_high & 0x02) y_high |= 0xFE;
	return ((y_high << 8) | y_low);}

short Acceleration::z()
{
	setupI2C();
	if(!s_setup) return 0xFFFF;
	short z_low = static_cast<short>(Private::I2C::instance()->read(R_ZOUTL));
	short z_high = static_cast<short>(Private::I2C::instance()->read(R_ZOUTH));
	if(z_high & 0x02) z_high |= 0xFE;
	return ((z_high << 8) | z_low);
}

void Acceleration::setupI2C()
{
	if(s_setup) return;
	s_setup = Private::I2C::instance()->pickSlave("0x1d");
	Private::I2C::instance()->write(DATA_ADDR, DATA_VAL, true);
}
