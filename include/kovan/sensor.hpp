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
 * \file sensor.hpp
 * \brief Base sensor classes
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup sensor Sensors
 */

#ifndef _SENSORS_HPP_
#define _SENSORS_HPP_

#include "export.h"

/*!
 * \class Sensor
 * \brief The base class for all sensors of any type
 * \tparam T The return type of this sensor.
 * For example, a sensor returning true or false should be of type bool.
 * \ingroup sensor
 */
template<typename T>
class EXPORT_SYM Sensor
{
public:
	virtual ~Sensor() {}
	/*!
	 * Get the current value for this sensor
	 * \return The sensor's current value
	 */
	virtual T value() const = 0;
};

#endif
