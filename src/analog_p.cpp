#include "analog_p.hpp"
#include "shm_p.hpp"

using namespace Private;

void Analog::setPullup(const unsigned char& port, const bool& pullup)
{
	if(port < 8 || port > 15) return;
	
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm) return;
	
	shm->analogPullupsDirty &= 1 << (NUM_ANALOGS - 1 - port);
	shm->analogPullups &= pullup << (NUM_ANALOGS - 1 - port);
	
	SharedMemoryImpl::instance()->doAutoPublish();
}

bool Analog::pullup(const unsigned char& port) const
{
	if(port >= NUM_ANALOGS) return false;
	
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	return shm->analogPullups & (NUM_ANALOGS - 1 - port);
}

unsigned short Analog::value(const unsigned char& port) const
{
	if(port >= NUM_ANALOGS) return 0xFFFF;
	
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	return shm->analogs[port];
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