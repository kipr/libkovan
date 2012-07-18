#include "servo.hpp"
#include "servo_p.hpp"

Servo::Servo(port_t port)
	: m_port(port)
{
	
}

void Servo::setPosition(Servo::ticks_t position)
{
	Private::Servo::instance()->setPosition(m_port, position);
}

Servo::ticks_t Servo::position() const
{
	return Private::Servo::instance()->position(m_port);
}