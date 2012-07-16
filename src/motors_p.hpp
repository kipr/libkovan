#ifndef _MOTORS_P_HPP_
#define _MOTORS_P_HPP_

#include "port.hpp"
#include "common_p.hpp"

namespace Private
{
	class Motor : private Dev
	{
	public:
		static Motor *motorForPort(const port_t& port);
		
		void setPid(const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd);
		void pid(short& p, short& i, short& d, short& pd, short& id, short& dd);
		void setPwm(int speed);
		int pwm();
		void stop();
	private:
		Motor(const char *dev);
		
		static Motor motors[4];
	};
}

#endif

