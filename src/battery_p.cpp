#include "battery_p.hpp"
#include "shm_p.hpp"
#include "private.hpp"

Private::Battery::level_t Private::Battery::level()
{
	Private::SharedMemory *shared = SharedMemoryImpl::sharedMemory();
	pthread_mutex_t *mutex = reinterpret_cast<pthread_mutex_t *>(&shared->server);
	pthread_mutex_lock(mutex);
	Private::Battery::level_t level = shared ? shared->server.rawBatteryVoltage : 0xFF;
	pthread_mutex_unlock(mutex);
	return level;
}

Private::Battery *Private::Battery::instance()
{
	static Battery s_instance;
	return &s_instance;
}

Private::Battery::Battery() {}