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

#ifndef _SERVO_P_HPP_
#define _SERVO_P_HPP_

#include "kovan/port.hpp"

namespace Private
{
	class Servo
	{
	public:
		~Servo();
		
		void setEnabled(port_t port, const bool &enabled);
		bool isEnabled(port_t port);
		
		bool setPosition(port_t port, const unsigned short& position);
		unsigned short position(port_t port) const;
		
		static Servo *instance();
	private:
		Servo();
		
		port_t fixPort(port_t port) const;
	};
}

#endif
