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

/*!
 * \file servo.hpp
 * \brief Classes for working with Servos
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup servo Servos
 */

#ifndef _SERVOS_HPP_
#define _SERVOS_HPP_

#include "port.hpp"
 #include "export.h"

/*!
 * \class Servo
 * \brief Encapsulates the concept of a servo motor
 * \ingroup servo
 */
class EXPORT_SYM Servo
{
public:
	typedef unsigned int ticks_t;
	
	/*!
	 * Create a new servo object.
	 * \param port The associated physical servo port
	 */
	Servo(port_t port);
	
	/*!
	 * \param position The new servo position, between 0 and 1024
	 *
	 * \note Even though the servos have a _theoretical_ range between 0 and 1023,
	 * the _actual_ range is often less. Setting the servo to a position that it cannot physically
	 * reach will cause the servo to audibly strain and will consume battery very quickly.
	 */
	void setPosition(ticks_t position);
	
	/*!
	 * \return The servo's position as a 10 bit integer
	 * \note Thie method will return the last _sent_ position, not the currently _set_ position.
	 * 
	 * \note For example, imagine the following:
	 * -# myServo.setPosition(700);
	 * -# ... Some time passes ...
	 * -# myServo.setPosition(300);
	 * -# myServo.position();
	 * 
	 * \note position() will return 700 rather than 300,
	 * because 300 hasn't been sent to the servo yet.
	 */
	ticks_t position() const;
	
	void disable();
	void enable();
	void setEnabled(const bool &enabled);
	bool isEnabled() const;
private:
	port_t m_port;
};

#endif
