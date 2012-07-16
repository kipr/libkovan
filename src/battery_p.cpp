#include "battery_p.hpp"

Private::Battery::level_t Private::Battery::level()
{
	if(!isOpen()) return 0xFFFF;
	Private::Battery::level_t ret = 0xFFFF;
	read(&ret);
	return ret;
}

Private::Battery *Private::Battery::instance()
{
	static Battery s_instance(DEV(battery));
	return &s_instance;
}

Private::Battery::Battery(const char *dev) : Dev(dev) { open(); }