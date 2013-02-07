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
 * \file motors.hpp
 * \brief Provides classes for working with motors.
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup motor Motors
 */

#ifndef _MOTORS_HPP_
#define _MOTORS_HPP_

#include "port.hpp"
#include "sensor.hpp"
#include "export.h"

/*!
 * \class Motor
 * \brief A motor object that is associated with a physical motor port.
 * \details Allows both PWM and PID control of a motor.
 * \ingroup motor
 */
class EXPORT_SYM Motor
{
public:
	/*!
	 * Creates a new motor object associated with a given physical port.
	 * \param port the port this motor will communicate with.
	 */
	Motor(const port_t& port) throw();
	
	void clearPositionCounter();
	
	void moveAtVelocity(const short& velocity);
	void moveToPosition(const short& speed, const int& goalPos);
	void moveRelativePosition(const short& speed, const int& deltaPos);
	
	void setPidGains(const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd);
	void pidGains(short& p, short& i, short& d, short& pd, short& id, short& dd);
	
	void freeze();
	
	/*!
	 * \return true if the motor has reached its target position, false otherwise
	 * \see blockMotorDone
	 */
	bool isMotorDone() const;
	
	/*!
	 * Waits until the motor has reached its target position.
	 * \see getMotorDone
	 * \blocksuntil the motor has reached its target position.
	 */
	void blockMotorDone() const;

	/*!
	 * Move the motor forward at full speed using PWM control.
	 * \see backward
	 * \see motor
	 */
	void forward();
	
	/*!
	 * Move the motor backward at full speed using PWM control.
	 * \see forward
	 * \see motor
	 */
	void backward();
	
	/*!
	 * Move the motor at a given percentage of power using PWM control.
	 * \param percent The speed to move the motor at, in percentage. Should be between -100% and 100%
	 */
	void motor(int percent);
	
	/*!
	 * Turns the motor off.
	 */
	void off();
	
	/*!
	 * The pyhsical port this motor is associated with.
	 * \return the port's address.
	 */
	const port_t& port() const;
	
private:
	port_t m_port;
};

/*!
 * \class BackEMF
 * \brief Allows the reading of the back emf values for each motor.
 * \ingroup motor
 */
class EXPORT_SYM BackEMF : public Sensor<int>
{
public:
	BackEMF(const unsigned char& port);
	virtual int value() const;
	unsigned char port() const;
	
private:
	unsigned char m_port;
};

#endif
