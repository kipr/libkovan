#include "servo.h"
#include "servo_p.hpp"

int get_servo_position(int servo)
{
	return Private::Servo::instance()->position(servo);
}

void set_servo_position(int servo, int position)
{
	Private::Servo::instance()->setPosition(servo, position);
}