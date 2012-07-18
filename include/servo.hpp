#ifndef _SERVOS_HPP_
#define _SERVOS_HPP_

#include "port.hpp"

class Servo
{
public:
	typedef unsigned int ticks_t;
	
	Servo(port_t port);
	
	void setPosition(ticks_t position);
	ticks_t position() const;
private:
	port_t m_port;
};

#endif
