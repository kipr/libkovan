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
 * \file servo.h
 * \brief Methods for working with Servos
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup servo Servos
 */

#ifndef _SERVOS_H_
#define _SERVOS_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM void enable_servo(int port);

EXPORT_SYM void disable_servo(int port);

EXPORT_SYM void enable_servos();

EXPORT_SYM void disable_servos();

EXPORT_SYM void set_servo_enabled(int port, int enabled);

EXPORT_SYM int get_servo_enabled(int port);


/*!
 * \param servo The port of the servo
 * \return The servo's position as a 10 bit integer (which is an integer between 0 and 1024)
 * \note Thie method will return the last _sent_ position, not the currently _set_ position.
 * 
 * \note For example, imagine the following:
 * -# set_servo_position(0, 700);
 * -# ... Some time passes ...
 * -# set_servo_position(0, 300);
 * -# get_servo_position(0);
 * 
 * \note get_servo_position(int) will return 700 rather than 300,
 * because 300 hasn't been sent to the servo yet.
 * \ingroup servo
 */
EXPORT_SYM int get_servo_position(int servo);

/*!
 * \param servo The port of the servo
 * \param position The new servo position, between 0 and 1023
 *
 * \note Even though the servos have a _theoretical_ range between 0 and 1024,
 * the _actual_ range is often less. Setting the servo to a position that it cannot physically
 * reach will cause the servo to audibly strain and will consume battery very quickly.
 * \ingroup servo
 */
EXPORT_SYM void set_servo_position(int servo, int position);

#ifdef __cplusplus
}
#endif


#endif
