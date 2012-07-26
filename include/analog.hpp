#ifndef _ANALOG_HPP_
#define _ANALOG_HPP_

#include "sensor.hpp"

class Analog : public Sensor<unsigned short>
{
public:
	Analog(const unsigned char& port);
	
	virtual unsigned short value() const;
	
	virtual void setPullup(bool pullup);
	virtual bool pullup() const;
	
	unsigned char port() const;
	
private:
	unsigned char m_port;
};

class Analog8 : public Analog
{
public:
	Analog8(const unsigned char& port);
	virtual unsigned short value() const;
};

#endif
