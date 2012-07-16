#include "util.h"
#include "time_p.hpp"

void msleep(long msecs)
{
	Private::Time::microsleep(msecs * 1000);
}