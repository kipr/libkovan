#include "motors.hpp"
#include "motors_p.hpp"

Motor::Motor(const port_t& port) throw()
	: m_port(port),
	p_motor(Private::Motor::motorForPort(port))
{
	if(!p_motor) throw InvalidPort("Motor ports are through 1 and 4");
}

void Motor::moveAtVelocity(const int& velocity)
{
	
}

void Motor::moveToPosition(const int& speed, const int& goalPos)
{
	
}

void Motor::moveRelativePosition(const int& speed, const int& deltaPos)
{
	
}

void Motor::setPidGains(const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd)
{
	p_motor->setPid(p, i, d, pd, id, dd);
}

void Motor::pidGains(short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	p_motor->pid(p, i, d, pd, id, dd);
}

void Motor::freeze()
{
	
}

bool Motor::getMotorDone() const
{
	return false;
}

void Motor::blockMotorDone() const
{
	
}

void Motor::forward()
{
	motor(100);
}

void Motor::backward()
{
	motor(-100);
}

void Motor::motor(int percent)
{
	p_motor->setPwm(percent);
}

void Motor::off()
{
	p_motor->stop();
}

const port_t& Motor::port() const
{
	return m_port;
}