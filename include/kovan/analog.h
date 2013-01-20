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

#ifndef _ANALOG_H_
#define _ANALOG_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Gets the 10-bit analog value of a port.
 * \param[in] port A value between 0 and 7 specifying the sensor to read from.
 * \return The latest 10-bit value of the port (a value in the range 0 to 1023).
 * \see analog
 * \ingroup sensor
 */
EXPORT_SYM int analog10(int port);

/*!
 * Gets the 8-bit analog value of a port.
 * \param[in] port A value between 0 and 7 specifying the sensor to read from.
 * \return The latest 8-bit value of the port (a value in the range 0 to 255).
 * \see analog10
 * \ingroup sensor
 */
EXPORT_SYM int analog(int port);

EXPORT_SYM void set_analog_pullup(int port, int pullup);
EXPORT_SYM int get_analog_pullup(int port);

#ifdef __cplusplus
}
#endif

#endif
