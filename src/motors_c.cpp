#include "motors.h"
#include "motors_p.hpp"

#include <iostream>

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
	return -1;
}

int mav(int motor, int velocity)
{
	return move_at_velocity(motor, velocity);
}

int move_to_position(int motor, int speed, int goal_pos)
{
	return -1;
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
	return -1;
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
	Private::Motor::instance()->setPwm(motor, percent);
	
	if(percent > 0) Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Forward);
	else if(percent < 0) Private::Motor::instance()->setPwmDirection(motor, Private::Motor::Reverse);
	else Private::Motor::instance()->setPwmDirection(motor, Private::Motor::PassiveStop);
}

void off(int motor)
{
	Private::Motor::instance()->stop(motor);
}

void ao()
{
	for(int i = 1; i <= 4; ++i) off(i);
}

int get_backemf(int port)
{
	return Private::Motor::instance()->backEMF(port);
}
