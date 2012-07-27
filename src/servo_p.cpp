#include "servo_p.hpp"
#include "shm_p.hpp"
#include "private.hpp"

namespace Private
{
	class ServoPublishListener : public PublishListener
	{
	public:
		virtual void published(Private::SharedMemoryClient *client)
		{
			client->servoDirty = 0;
		}
	};
}

Private::Servo::Servo()
	: m_listener(new ServoPublishListener())
{
	SharedMemoryImpl::instance()->addPublishListener(m_listener);
}

Private::Servo::~Servo()
{
	delete m_listener;
}

bool Private::Servo::setPosition(const port_t& port, const unsigned short& position)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm) return false;
	if(port < 1 || port > 4) return false;
	shm->servoPositions[port - 1] = position & 0x3FF;
	shm->servoDirty |= 1 << (4 - port);
	return true;
}

unsigned short Private::Servo::position(const port_t& port) const
{
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm) return false;
	if(port < 1 || port > 4) return false;
	unsigned short pos = shm->servoPositions[port - 1] & 0x3FF;
	return pos;
}

Private::Servo *Private::Servo::instance()
{
	static Servo instance;
	return &instance;
}