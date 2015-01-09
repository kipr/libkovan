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

VH #include "kovan/motors.h"
VH #include "kovan/util.h"
#include "motors_p.hpp"

#include <iostream>
#include <cstdlib>
#include <math.h>

VI int get_motor_position_counter(int motor)
{
	return Private::Motor::instance()->backEMF(motor);
}

VI void gmpc(int motor)
{
  get_motor_position_counter(motor);
}

VI void clear_motor_position_counter(int motor)
{
	Private::Motor::instance()->clearBemf(motor);
}

VI void cmpc(int motor)
{
  clear_motor_position_counter(motor);
}

VI int move_at_velocity(int motor, int velocity)
{
	Private::Motor::instance()->setControlMode(motor, Private::Motor::Speed);
	Private::Motor::instance()->setPidVelocity(motor, velocity);
	return 0;
}

VI int mav(int motor, int velocity)
{
	return move_at_velocity(motor, velocity);
}

VI int move_to_position(int motor, int speed, int goal_pos)
{
	short velocity = std::abs(speed);
	const int sign = Private::Motor::instance()->backEMF(motor) > goal_pos ? -1 : 1;
	velocity *= sign;
	Private::Motor::instance()->setControlMode(motor, Private::Motor::SpeedPosition);
	Private::Motor::instance()->setPidGoalPos(motor, goal_pos);
	Private::Motor::instance()->setPidVelocity(motor, velocity);
	return 0;
}

VI int mtp(int motor, int speed, int goal_pos)
{
	return move_to_position(motor, speed, goal_pos);
}

VI int move_relative_position(int motor, int speed, int delta_pos)
{
	if(motor < 0 || motor > 3) return -1;
	move_to_position(motor, speed, Private::Motor::instance()->backEMF(motor) + delta_pos);
	return 0;
}

VI int mrp(int motor, int speed, int delta_pos)
{
	return move_relative_position(motor, speed, delta_pos);
}

VI void set_pid_gains(int motor, short p, short i, short d, short pd, short id, short dd)
{
	Private::Motor::instance()->setPidGains(motor, p, i, d, pd, id, dd);
}

VI void get_pid_gains(int motor, short *p, short *i, short *d, short *pd, short *id, short *dd)
{
	Private::Motor::instance()->pidGains(motor, *p, *i, *d, *pd, *id, *dd);
}

VI int freeze(int motor)
{
	Private::Motor::instance()->setPwm(motor, 100);
	Private::Motor::instance()->setPwmDirection(motor, Private::Motor::ActiveStop);
	return 0;
}

VI int get_motor_done(int motor)
{
	if(motor < 0 || motor > 3) return -1;
	// This sleep is necessary to make sure the PID control loop has run
	msleep(50);
	return Private::Motor::instance()->isPidActive(motor) ? 0 : 1;
}

VI void block_motor_done(int motor)
{
	while(!get_motor_done(motor));
}

VI void bmd(int motor)
{
	block_motor_done(motor);
}

VI int setpwm(int motor, int pwm)
{
	Private::Motor::instance()->setPwm(motor, pwm);
	return -1;
}

VI int getpwm(int motor)
{
	return -1;
}

VI void fd(int motor)
{
	::motor(motor, 100);
}

VI void bk(int motor)
{
	::motor(motor, -100);
}

VI void motor(int motor, int percent)
{
	Private::Motor::instance()->setPwm(motor, std::abs(percent));
	
	if(percent > 0) Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Forward);
	else if(percent < 0) Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Reverse);
	else Private::Motor::instance()->setPwmDirection(motor, Private::Motor::PassiveStop);
}

VI void off(int motor)
{
	Private::Motor::instance()->stop(motor);
}

VI void alloff()
{
	ao();
}

VI void ao()
{
	for(unsigned char i = 0; i < 4; ++i) off(i);
}