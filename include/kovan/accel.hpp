#ifndef _ACCEL_HPP_
#define _ACCEL_HPP_

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

#endif
