#ifndef _MOTORS_P_HPP_
#define _MOTORS_P_HPP_

#include "port.hpp"

namespace Private
{
	struct PID;
	class PublishListener;
	
	class Motor
	{
	public:
		enum ControlMode {
			Unknown,
			PID,
			PWM
		};
		
		enum Direction {
			PassiveStop = 0,
			Reverse,
			Forward,
			ActiveStop
		};
		
		~Motor();
		
		void setPid(const port_t& port, const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd);
		
		void setControlMode(const Motor::ControlMode& mode);
		Motor::ControlMode controlMode() const;
		
		void pid(const port_t& port, short& p, short& i, short& d, short& pd, short& id, short& dd);
		
		void setPwm(const port_t& port, unsigned char speed);
		void setPwmDirection(const port_t& port, const Motor::Direction& dir);
		
		unsigned char pwm(const port_t& port);
		Motor::Direction pwmDirection(const port_t& port) const;
		
		void stop(const port_t& port);
		
		unsigned short backEMF(const unsigned char& port);
		
		static Motor *instance();
	private:
		
		Motor();
		
		PublishListener *m_listener;
	};
}

#endif

