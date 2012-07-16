#include "motors_p.hpp"
#include <iostream> // TMP

Private::Motor Private::Motor::motors[4] = {
	Private::Motor(DEV(motor0)),
	Private::Motor(DEV(motor1)),
	Private::Motor(DEV(motor2)),
	Private::Motor(DEV(motor3))
};

Private::Motor *Private::Motor::motorForPort(const port_t& port)
{
	return (port > 0 && port < 5) ? &motors[port - 1] : 0;
}

void Private::Motor::setPid(const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd)
{
	std::cout << "Private::Motor::setPid(" << p << ", " << i << ", " << d << ", "
		<< pd << ", " << id << ", " << dd << ") -> " << dev() << std::endl;
}

void Private::Motor::pid(short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	p = i = d = pd = id = dd = 0;
}

void Private::Motor::setPwm(int speed)
{
	std::cout << "Private::Motor::setPwm(" << speed << ") -> " << dev() << std::endl;
}

int Private::Motor::pwm()
{
	return -1;
}

void Private::Motor::stop()
{
	std::cout << "Private::Motor::stop() -> " << dev() << std::endl;
}

Private::Motor::Motor(const char *dev) : Dev(dev)
{
	open();
}
