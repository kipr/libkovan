#ifndef _MOTORS_HPP_
#define _MOTORS_HPP_

#include "port.hpp"
#include "sensor.hpp"

namespace Private
{
	class Motor;
}

class Motor
{
public:
	Motor(const port_t& port) throw();
	
	void moveAtVelocity(const int& velocity);
	void moveToPosition(const int& speed, const int& goalPos);
	void moveRelativePosition(const int& speed, const int& deltaPos);
	
	void setPidGains(const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd);
	void pidGains(short& p, short& i, short& d, short& pd, short& id, short& dd);
	
	void freeze();
	
	bool getMotorDone() const;
	void blockMotorDone() const;
	
	void forward();
	void backward();
	void motor(int percent);
	void off();
	
	const port_t& port() const;
private:
	port_t m_port;
};

class BackEMF : public Sensor<unsigned short>
{
public:
	BackEMF(const unsigned char& port);
	virtual unsigned short value() const;
	unsigned char port() const;
	
private:
	unsigned char m_port;
};

#endif
