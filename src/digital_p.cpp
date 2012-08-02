#include "digital_p.hpp"
#include "shm_p.hpp"

using namespace Private;

bool Digital::value(const unsigned char& port) const
{
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedServer();
	if(!shm || port >= NUM_DIGITALS) return false;
	return shm->digitals & (1 << (NUM_DIGITALS - 1 - port));
}

bool Digital::setValue(const unsigned char& port, const bool& value)
{
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedClient();
	if(!shm || port >= NUM_DIGITALS) return false;
	
	shm->digitalsDirty &= 1 << (NUM_DIGITALS - 1 - port);
	shm->digitals &= value << (NUM_DIGITALS - 1 - port);
	SharedMemoryImpl::instance()->doAutoPublish();
}

const Digital::Direction Digital::direction(const unsigned char& port) const
{
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedServer();
	if(!shm || port >= NUM_DIGITALS) return false;
	return shm->digitalDirections & (1 << (NUM_DIGITALS - 1 - port)) ? Out : In;
}

bool Digital::setDirection(const unsigned char& port, const Digital::Direction& direction)
{
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedClient();
	if(!shm || port >= NUM_DIGITALS) return false;
	
	shm->digitalDirectionsDirty &= 1 << (NUM_DIGITALS - 1 - port);
	shm->digitalDirections &= (direction == In ? 0 : 1) << (NUM_DIGITALS - 1 - port);
	SharedMemoryImpl::instance()->doAutoPublish();
}

bool Digital::pullup(const unsigned char& port) const
{
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedServer();
	if(!shm || port > NUM_DIGITALS - 1) return false;
	return shm->digitalPullups & (1 << (NUM_DIGITALS - 1 - port));
}

bool Digital::setPullup(const unsigned char& port, const bool& pullup)
{
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedClient();
	if(!shm || port >= NUM_DIGITALS) return false;
	
	shm->digitalPullupsDirty &= 1 << (NUM_DIGITALS - 1 - port);
	shm->digitalPullups &= (direction == In ? 0 : 1) << (NUM_DIGITALS - 1 - port);
	SharedMemoryImpl::instance()->doAutoPublish();
}

Digital::Digital() {}
Digital::Digital(const Digital&) {}
Digital& Digital::operator=(const Digital&) { return *this; }