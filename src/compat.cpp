#include "kovan/compat.hpp"

int compat::yield()
{
#ifdef _MSC_VER
	Sleep(0);
	return 0;
#else
	return sched_yield();
#endif
}

int compat::microsleep(unsigned long microseconds)
{
#ifdef _MSC_VER
	Sleep(microseconds / 1000L);
	return 0;
#else
	return usleep(microseconds);
#endif
}