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

#include "kovan/servo.h"
#include "servo_p.hpp"

void enable_servo(int servo)
{
	set_servo_enabled(servo, 1);
}

void disable_servo(int servo)
{
	set_servo_enabled(servo, 0);
}

void enable_servos()
{
	for(int i = 0; i < 4; ++i) enable_servo(i);
}

void disable_servos()
{
	for(int i = 0; i < 4; ++i) disable_servo(i);
}

void set_servo_enabled(int servo, int enabled)
{
	Private::Servo::instance()->setEnabled(servo, enabled);
}

int get_servo_enabled(int servo)
{
	return Private::Servo::instance()->isEnabled(servo) ? 1 : 0;
}

int get_servo_position(int servo)
{
	return Private::Servo::instance()->position(servo);
}

void set_servo_position(int servo, int position)
{
	Private::Servo::instance()->setPosition(servo, position);
}