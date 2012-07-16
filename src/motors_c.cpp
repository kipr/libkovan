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
	Private::Motor *p_motor = Private::Motor::motorForPort(motor);
	if(!p_motor) {
		std::cerr << "Invalid port" << std::endl;
		return 0;
	} else p_motor->setPwm(pwm);
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
	Private::Motor *p_motor = Private::Motor::motorForPort(motor);
	if(!p_motor) std::cerr << "Invalid port" << std::endl;
	else p_motor->setPwm(percent);
}

void off(int motor)
{
	Private::Motor *p_motor = Private::Motor::motorForPort(motor);
	if(!p_motor) std::cerr << "Invalid port" << std::endl;
	else p_motor->stop();
}

void ao()
{
	for(int i = 1; i <= 4; ++i) off(i);
}
