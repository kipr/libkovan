#include "servo_p.hpp"
#include "shm_p.hpp"
#include "private.hpp"

bool Private::Servo::setPosition(const port_t& port, const unsigned short& position)
{
	Private::SharedMemory *shm = SharedMemoryImpl::sharedMemory();
	if(!shm) return false;
	if(port < 1 || port > 4) return false;
	pthread_mutex_t *mutex = reinterpret_cast<pthread_mutex_t *>(&shm->client); // First member of shm, so we can just cast it
	pthread_mutex_lock(mutex);
	shm->client.servoPositions[port - 1] = position & 0x3F;
	shm->client.servoDirty |= 1 << (4 - port);
	pthread_mutex_unlock(mutex);
	return true;
}

unsigned short Private::Servo::position(const port_t& port) const
{
	Private::SharedMemory *shm = SharedMemoryImpl::sharedMemory();
	if(!shm) return false;
	if(port < 1 || port > 4) return false;
	pthread_mutex_t *mutex = reinterpret_cast<pthread_mutex_t *>(&shm->server);
	pthread_mutex_lock(mutex);
	unsigned short pos = shm->server.servoPositions[port - 1] & 0x3F;
	pthread_mutex_unlock(mutex);
	return pos;
}

Private::Servo *Private::Servo::instance()
{
	static Servo instance;
	return &instance;
}