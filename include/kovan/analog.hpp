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
 * \file analog.hpp
 * \brief Classes for working with analog sensors
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \ingroup sensor
 */

#ifndef _ANALOG_HPP_
#define _ANALOG_HPP_

#include "sensor.hpp"

#include "export.h"

/*!
 * \class Analog
 * \brief Implements the sensor type "analog"
 * \details Returns 10 bit unsigned integers
 * \see Analog8
 * \author Braden McDorman
 * \ingroup sensor
 */
class EXPORT_SYM  Analog : public Sensor<unsigned short>
{
public:
	Analog(const unsigned char& port);
	
	virtual unsigned short value() const;
	
	virtual void setPullup(bool pullup);
	virtual bool pullup() const;
	
	/*!
	 * Gets the physical port associated with this analog instance
	 * \return physical port address
	 */
	unsigned char port() const;
	
private:
	unsigned char m_port;
};

/*!
 * \class Analog8
 * \brief A derivative of the Analog class that returns 8 bit values rather than 10 bit ones.
 * \details Returns 8 bit unsigned integers
 * \see Analog
 * \author Braden McDorman
 * \ingroup sensor
 */
class EXPORT_SYM Analog8 : public Analog
{
public:
	Analog8(const unsigned char& port);
	virtual unsigned short value() const;
};

#endif
