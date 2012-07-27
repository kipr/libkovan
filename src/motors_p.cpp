#include "motors_p.hpp"
#include "shm_p.hpp"
#include <iostream> // TMP

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

Motor::ControlMode Private::Motor::controlMode() const
{
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm) return Unknown;
	
	switch(shm->motorControlMode) {
	case Private::PIDMode: return PID;
	case Private::PWMMode: return PWM;
	}
	return Private::Unknown;
}

void Private::Motor::setPid(const port_t& port, const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd)
{
	
	Private::PID *pid = clientSharedPID(port);
	if(!pid) return;
	
	pid->p = p;
	pid->i = i;
	pid->d = d;
	pid->pd = pd;
	pid->id = id;
	pid->dd = dd;
}

void Private::Motor::pid(const port_t& port, short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	Private::PID *pid = clientSharedPID(port);
	if(!pid) return;
	
	p = pid->p;
	i = pid->i;
	d = pid->d;
	pd = pid->pd;
	id = pid->id;
	dd = pid->dd;
}

void Private::Motor::setPwm(const port_t& port, int speed)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port > 3) return;
	shm->pwms[port] = speed;
}

char Private::Motor::pwm(const port_t& port)
{
	return -1;
}

void Private::Motor::stop(const port_t& port)
{
	std::cout << "Private::Motor::stop() -> " << std::endl;
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
{
}

Private::PID *Private::Motor::clientSharedPID(const port_t& port) const
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port > 3) return 0;
	return &shm->pids[port];
}

Private::PID *Private::Motor::serverSharedPID(const port_t& port) const
{
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm || port > 3) return 0;
	return &shm->pids[port];
}