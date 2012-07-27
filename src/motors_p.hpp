#ifndef _MOTORS_P_HPP_
#define _MOTORS_P_HPP_

#include "port.hpp"

namespace Private
{
	class Motor
	{
	public:
		void setPid(const port_t& port, const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd);
		void pid(const port_t& port, short& p, short& i, short& d, short& pd, short& id, short& dd);
		void setPwm(const port_t& port, int speed);
		int pwm(const port_t& port);
		void stop(const port_t& port);
		
		unsigned short backEMF(const unsigned char& port);
		
		static Motor *instance();
	private:
		Motor();
	};
}

#endif

