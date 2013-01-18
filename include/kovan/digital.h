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
 * \file digital.h
 * \brief Functions for working with digital sensors
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \ingroup sensor
 */

#ifndef _DIGITAL_H_
#define _DIGITAL_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \see get_digital_value
 */
EXPORT_SYM int digital(int port);

/*!
 * Sets the value of the digital port in output mode.
 * \see get_digital_value
 */
EXPORT_SYM void set_digital_value(int port, int value);

/*!
 * Gets the current value of the digital port.
 * \return 1 if the switch is closed, 0 if the switch is open
 * \see set_digital_value
 */
EXPORT_SYM int get_digital_value(int port);

/*!
 * Sets the digital mode.
 * \param port The port to modify.
 * \param out 1 for output mode, 0 for input mode.
 */
EXPORT_SYM void set_digital_output(int port, int out);
EXPORT_SYM int get_digital_output(int port);

EXPORT_SYM int get_digital_pullup(int port);
EXPORT_SYM void set_digital_pullup(int port, int pullup);

#ifdef __cplusplus
}
#endif

#endif
