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
 * \file digital.hpp
 * \brief Classes for working with digital sensors
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \ingroup sensor
 */

#ifndef _DIGITAL_HPP_
#define _DIGITAL_HPP_

#include "sensor.hpp"
#include "export.h" 

/*!
 * \class Digital
 * \brief Facilitates access to a digital sensor
 * \author Braden McDorman
 * \ingroup sensor
 */
class EXPORT_SYM Digital : public Sensor<bool>
{
public:
	Digital(const unsigned char& port);
	
	virtual bool setValue(const bool& value);
	
	/*!
	 * Gets the current value of the digital sensor
	 */
	virtual bool value() const;
	
	virtual void setOutput(const bool& output);
	virtual bool isOutput() const;
	
	virtual void setPullup(const bool& pullup);
	virtual bool pullup() const;
private:
	unsigned char m_port;
};

#endif
