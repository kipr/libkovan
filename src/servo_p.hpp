#ifndef _SERVO_P_HPP_
#define _SERVO_P_HPP_

#include "port.hpp"

namespace Private
{
	class Servo
	{
	public:	
		bool setPosition(const port_t& port, const unsigned short& position);
		unsigned short position(const port_t& port) const;
		
		static Servo *instance();
	private:
	};
}

#endif
