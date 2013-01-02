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

#include "kovan/motors.h"
#include "motors_p.hpp"

#include <iostream>
#include <cstdlib>

int get_motor_position_counter(int motor)
{
	return -1;
}

int clear_motor_position_counter(int motor)
{
	return -1;
}

int move_at_velocity(int motor, int velocity)
{
	Private::Motor::instance()->setPidVelocity(motor, velocity, false);
	return 0;
}

int mav(int motor, int velocity)
{
	return move_at_velocity(motor, velocity);
}

int move_to_position(int motor, int speed, int goal_pos)
{
	Private::Motor::instance()->setPidGoalPos(motor, goal_pos);
	Private::Motor::instance()->setPidVelocity(motor, speed, true);
	return 0;
}

int mtp(int motor, int speed, int goal_pos)
{
	return move_to_position(motor, speed, goal_pos);
}

int move_relative_position(int motor, int speed, int delta_pos)
{
	return -1;
}

int mrp(int motor, int speed, int delta_pos)
{
	return move_relative_position(motor, speed, delta_pos);
}

void set_pid_gains(int motor, int p, int i, int d, int pd, int id, int dd)
{
	
}

void get_pid_gains(int motor, int *p, int *i, int *d, int *pd, int *id, int *dd)
{
	
}

int freeze(int motor)
{
	Private::Motor::instance()->setPwm(motor, 100);
	Private::Motor::instance()->setPwmDirection(motor, Private::Motor::ActiveStop);
	return 0;
}

int get_motor_done(int motor)
{
	return -1;
}

void block_motor_done(int motor)
{
	
}

void bmd(int motor)
{
	block_motor_done(motor);
}

int setpwm(int motor, int pwm)
{
	Private::Motor::instance()->setPwm(motor, pwm);
	return -1;
}

int getpwm(int motor)
{
	return -1;
}

void fd(int motor)
{
	::motor(motor, 100);
}

void bk(int motor)
{
	::motor(motor, -100);
}

void motor(int motor, int percent)
{
	Private::Motor::instance()->setPwm(motor, std::abs(percent));
	
	if(percent > 0) Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Forward);
	else if(percent < 0) Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Reverse);
	else Private::Motor::instance()->setPwmDirection(motor, Private::Motor::PassiveStop);
}

void off(int motor)
{
	Private::Motor::instance()->stop(motor);
}

void alloff()
{
	ao();
}

void ao()
{
	for(unsigned char i = 0; i < 4; ++i) off(i);
}

int get_backemf(int port)
{
	return Private::Motor::instance()->backEMF(port);
}
