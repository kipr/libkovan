#include "analog.hpp"
#include "analog_p.hpp"

Analog::Analog(const unsigned char& port)
	: m_port(port)
{
	
}

unsigned short Analog::value() const
{
	return Private::Analog::instance()->value(m_port);
}

void Analog::setPullup(bool pullup)
{
	return Private::Analog::instance()->setPullup(m_port, pullup);
}

bool Analog::pullup() const
{
	return Private::Analog::instance()->pullup(m_port);
}

Analog8::Analog8(const unsigned char& port) : Analog(port) {}

unsigned short Analog8::value() const
{
	return Analog::value() >> 2;
}
