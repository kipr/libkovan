#include "motors_p.hpp"
#include "shm_p.hpp"
#include <iostream> // TMP

void Private::Motor::setPid(const port_t& port, const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd)
{
	std::cout << "Private::Motor::setPid(" << p << ", " << i << ", " << d << ", "
		<< pd << ", " << id << ", " << dd << ") -> " << std::endl;
}

void Private::Motor::pid(const port_t& port, short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	p = i = d = pd = id = dd = 0;
}

void Private::Motor::setPwm(const port_t& port, int speed)
{
	std::cout << "Private::Motor::setPwm(" << speed << ") -> " << std::endl;
}

int Private::Motor::pwm(const port_t& port)
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
