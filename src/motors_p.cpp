#include "motors_p.hpp"
#include "shm_p.hpp"
#include <iostream> // TMP

namespace Private
{
	class MotorPublishListener : public PublishListener
	{
	public:
		virtual void published(Private::SharedMemoryClient *client)
		{
			client->pidDirty = 0;
			client->pwmDirty = 0;
			client->motorDirectionsDirty = 0;
		}
	};
}

Private::Motor::~Motor()
{
	delete m_listener;
}

void Private::Motor::setControlMode(const Motor::ControlMode& mode)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm) return;
	
	switch(mode) {
	case PID: shm->motorControlMode = Private::PIDMode; break;
	case PWM: shm->motorControlMode = Private::PWMMode; break;
	case Unknown: break;
	}
}

Private::Motor::ControlMode Private::Motor::controlMode() const
{
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm) return Unknown;
	
	switch(shm->motorControlMode) {
	case Private::PIDMode: return PID;
	case Private::PWMMode: return PWM;
	}
	return Unknown;
}

void Private::Motor::setPid(const port_t& port, const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd)
{
	
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm || port > 3) return;
	Private::PID *pid = &shm->pids[port];
	
	pid->p = p;
	pid->i = i;
	pid->d = d;
	pid->pd = pd;
	pid->id = id;
	pid->dd = dd;
}

void Private::Motor::pid(const port_t& port, short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port > 3) return;
	
	Private::PID *pid = &shm->pids[port];
	p = pid->p;
	i = pid->i;
	d = pid->d;
	pd = pid->pd;
	id = pid->id;
	dd = pid->dd;
	
	shm->pidDirty |= 1 << (3 - port);
}

void Private::Motor::setPwm(const port_t& port, unsigned char speed)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port > 3) return;
	shm->pwms[port] = speed;
	shm->pwmDirty = 1 << (3 - port);
}

void Private::Motor::setPwmDirection(const port_t& port, const Motor::Direction& dir)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port > 3) return;

	// Clear direction
	const size_t shift = (3 - port) << 1;
	shm->motorDirections &= ~(0x3 << shift);

	printf("motorDirection = %x (port = %u, direction = %u)\n", shm->motorDirections, port, dir);

	switch(dir) {
	case Forward: shm->motorDirections |= Private::MotorDirection::Forward << shift; break;
	case Reverse: shm->motorDirections |= Private::MotorDirection::Reverse << shift; break;
	case PassiveStop: shm->motorDirections |= Private::MotorDirection::PassiveStop << shift; break;
	case ActiveStop: shm->motorDirections |= Private::MotorDirection::ActiveStop << shift; break;
	}
	
	shm->motorDirectionsDirty = true;
}

unsigned char Private::Motor::pwm(const port_t& port)
{
	return -1;
}

void Private::Motor::stop(const port_t& port)
{
	setPwmDirection(port, PassiveStop);
}

unsigned short Private::Motor::backEMF(const unsigned char& port)
{
	if(port >= 8) return 0xFFFF;
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm) return 0xFFFF;
	return shm->backEMFs[port];
}

Private::Motor *Private::Motor::instance()
{
	static Motor s_motor;
	return &s_motor;
}

Private::Motor::Motor()
	: m_listener(new Private::MotorPublishListener())
{
	SharedMemoryImpl::instance()->addPublishListener(m_listener);
}
