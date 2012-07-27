#include "analog_p.hpp"
#include "shm_p.hpp"

using namespace Private;

void Analog::setPullup(const unsigned char& port, const bool& pullup)
{
	if(port < 8 || port > 15) return;
	
	SharedMemory *shm = SharedMemoryImpl::instance()->sharedMemory();
	if(!shm) return;
	
	pthread_mutex_lock(&shm->clientMutex);
	shm->client.pullupDirty[port - 8] = true;
	shm->client.pullup[port - 8] = pullup;
	pthread_mutex_unlock(&shm->clientMutex);
}

bool Analog::pullup(const unsigned char& port) const
{
	if(port < 8 || port > 15) return false;
	
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	return shm->pullup[port - 8];
}

unsigned short Analog::value(const unsigned char& port) const
{
	if(port < 8 || port > 15) return 0xFFFF;
	
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	return shm->analogs[port - 8];
}

Analog *Analog::instance()
{
	static Analog s_analog;
	return &s_analog;
}

Analog::Analog()
{
	
}

Analog::Analog(const Analog& ) {}
Analog& Analog::operator=(const Analog&) { return *this; }