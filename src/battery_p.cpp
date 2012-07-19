#include "battery_p.hpp"
#include "shm_p.hpp"
#include "private.hpp"

Private::Battery::level_t Private::Battery::level()
{
	Private::SharedMemoryServer *shared = SharedMemoryImpl::instance()->sharedMemoryServer();
	return shared ? shared->rawBatteryVoltage : 0;
}

Private::Battery *Private::Battery::instance()
{
	static Battery s_instance;
	return &s_instance;
}

Private::Battery::Battery() {}