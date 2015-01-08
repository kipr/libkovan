VH #include "kovan/accel.h"
VH #include "kovan/accel.hpp"

VI signed short accel_x()
{
	return Acceleration::x();
}

VI signed short accel_y()
{
	return Acceleration::y();
}

VI signed short accel_z()
{
	return Acceleration::z();
}

VI int accel_calibrate()
{
	return Acceleration::calibrate() ? 0 : -1;
}

