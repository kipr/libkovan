#ifndef _ACCEL_HPP_
#define _ACCEL_HPP_

class Acceleration
{
public:
	static short x();
	static short y();
	static short z();
private:
	static void setupI2C();
};

#endif
