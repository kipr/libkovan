#include "time_p.hpp"
#include <unistd.h>

void Private::Time::microsleep(const long& microsecs)
{
	usleep(microsecs);
}