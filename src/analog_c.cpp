#include "analog.h"
#include "analog_p.hpp"

int analog10(int port)
{
	return Private::Analog::instance()->value(static_cast<unsigned char>(port));
}

int analog(int port)
{
	return analog10(port) >> 2;
}

void analog_set_pullup(int port, int pullup)
{
	Private::Analog::instance()->setPullup(static_cast<unsigned char>(port), pullup == 0 ? false : true);
}

int analog_get_pullup(int port)
{
	return Private::Analog::instance()->pullup(static_cast<unsigned char>(port));
}