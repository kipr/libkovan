#include "kovan/accel.h"
#include "kovan/accel.hpp"

short accel_x()
{
	return Acceleration::x();
}

short accel_y()
{
	return Acceleration::y();
}

short accel_z()
{
	return Acceleration::z();
}

int accel_calibrate()
{
	short success = (short) Acceleration::calibrate();

	if (success) return 0;

	return -1;
}
