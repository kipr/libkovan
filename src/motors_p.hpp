/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#ifndef _MOTORS_P_HPP_
#define _MOTORS_P_HPP_

#include "kovan/port.hpp"

namespace Private
{
	class Motor
	{
	public:
		enum ControlMode {
			Inactive = 0,
			Position = 1,
			Speed = 2,
			SpeedPosition = 3
		};
		
		enum Direction {
			PassiveStop = 0,
			Reverse,
			Forward,
			ActiveStop
		};
		
		~Motor();
		
		void setPidGains(const short &p, const short &i, const short &d,
			const short &pd, const short &id, const short &dd);
		
		void clearBemf(unsigned char port);
		
		void setControlMode(port_t port, Motor::ControlMode mode);
		Motor::ControlMode controlMode(port_t port) const;
		
		bool isPidActive(port_t port) const;
		
		void setPidVelocity(const port_t &port, const int &ticks);
		int pidVelocity(const port_t &port) const;
		
		void setPidGoalPos(const port_t &port, const int &pos);
		int pidGoalPos(const port_t &port) const;
		
		void setControlMode(const Motor::ControlMode &mode);
		Motor::ControlMode controlMode() const;
		
		void pidGains(short &p, short &i, short &d, short &pd, short &id, short &dd);
		
		void setPwm(const port_t &port, const unsigned char &speed);
		void setPwmDirection(const port_t &port, const Motor::Direction &dir);
		
		unsigned char pwm(const port_t &port);
		Motor::Direction pwmDirection(const port_t &port) const;
		
		void stop(const port_t &port);
		
		int backEMF(const unsigned char &port);
		
		static Motor *instance();
		
	private:
		Motor();
	};
}

#endif

