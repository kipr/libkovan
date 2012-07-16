#include "battery.hpp"
#include "battery_p.hpp"

float Battery::powerLevel()
{
	Private::Battery::level_t level = Private::Battery::instance()->level();
	return level;
}