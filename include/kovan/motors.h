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
 * \file motors.h
 * \brief Provides methods for working with motors.
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup motor Motors
 */

#ifndef _MOTORS_H_
#define _MOTORS_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ingroup motor
 */
EXPORT_SYM int get_motor_position_counter(int motor);

/*!
 * \ingroup motor
 */
EXPORT_SYM void clear_motor_position_counter(int motor);

/*!
 * \ingroup motor
 */
EXPORT_SYM int move_at_velocity(int motor, int velocity);

/*!
 * \see move_at_velocity
 * \ingroup motor
 */
EXPORT_SYM int mav(int motor, int velocity);

EXPORT_SYM int move_to_position(int motor, int speed, int goal_pos);

/*!
 * \see move_to_position
 * \ingroup motor
 */
EXPORT_SYM int mtp(int motor, int speed, int goal_pos);

EXPORT_SYM int move_relative_position(int motor, int speed, int delta_pos);

/*!
 * \see move_relative_position
 * \ingroup motor
 */
EXPORT_SYM int mrp(int motor, int speed, int delta_pos);

/*!
 * \ingroup motor
 */
EXPORT_SYM void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd);

/*!
 * \ingroup motor
 */
EXPORT_SYM void get_pid_gains(int motor, short *p, short *i, short *d, short *pd, short *id, short *dd);

/*!
 * \ingroup motor
 */
EXPORT_SYM int freeze(int motor);

/*!
 * \ingroup motor
 */
EXPORT_SYM int get_motor_done(int motor);

/*!
 * \ingroup motor
 */
EXPORT_SYM void block_motor_done(int motor);

/*!
 * \see block_motor_done
 * \ingroup motor
 */
EXPORT_SYM void bmd(int motor);

/*!
 * \ingroup motor
 */
EXPORT_SYM int setpwm(int motor, int pwm);

/*!
 * \ingroup motor
 */
EXPORT_SYM int getpwm(int motor);

/*!
 * Moves the given motor forward at full power
 * \param motor the motor's port.
 * \ingroup motor
 */
EXPORT_SYM void fd(int motor);

/*!
 * Moves the given motor backward at full power
 * \param motor the motor's port.
 * \ingroup motor
 */
EXPORT_SYM void bk(int motor);

/*!
 * \ingroup motor
 */
EXPORT_SYM void motor(int motor, int percent);

/*!
 * Turns the specified motor off.
 * \param motor the motor's port.
 * \ingroup motor
 */
EXPORT_SYM void off(int motor);

/*!
 * Turns all motors off.
 * \see ao
 * \ingroup motor
 */
EXPORT_SYM void alloff();

/*!
 * Turns all motors off.
 * \ingroup motor
 */
EXPORT_SYM void ao();


#ifdef __cplusplus
}
#endif


#endif
