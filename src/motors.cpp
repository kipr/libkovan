#include "motors.hpp"
#include "motors_p.hpp"

Motor::Motor(const port_t& port) throw()
	: m_port(port)
{
	// if(!p_motor) throw InvalidPort("Motor ports are through 1 and 4");
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
	Private::Motor::instance()->setPid(m_port, p, i, d, pd, id, dd);
}

void Motor::pidGains(short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	Private::Motor::instance()->pid(m_port, p, i, d, pd, id, dd);
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
	Private::Motor::instance()->setPwm(m_port, percent);
	if(percent > 0) Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::Forward);
	else if(percent < 0) Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::Reverse);
	else Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::PassiveStop);
}

void Motor::off()
{
	Private::Motor::instance()->stop(m_port);
}

const port_t& Motor::port() const
{
	return m_port;
}

BackEMF::BackEMF(const unsigned char& port) : m_port(port) {}

unsigned short BackEMF::value() const
{
	return Private::Motor::instance()->backEMF(m_port);
}

unsigned char BackEMF::port() const
{
	return m_port;
}
