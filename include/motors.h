/*!
 * \file motors.h
 * \brief Provides classes for working with motors.
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup motor Motors
 */

#ifndef _MOTORS_H_
#define _MOTORS_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \ingroup motor
 */
int get_motor_position_counter(int motor);

/*!
 * \ingroup motor
 */
int clear_motor_position_counter(int motor);

/*!
 * \ingroup motor
 */
int move_at_velocity(int motor, int velocity);

/*!
 * \see move_at_velocity
 * \ingroup motor
 */
int mav(int motor, int velocity);

int move_to_position(int motor, int speed, int goal_pos);

/*!
 * \see move_to_position
 * \ingroup motor
 */
int mtp(int motor, int speed, int goal_pos);

int move_relative_position(int motor, int speed, int delta_pos);

/*!
 * \see move_relative_position
 * \ingroup motor
 */
int mrp(int motor, int speed, int delta_pos);

/*!
 * \ingroup motor
 */
void set_pid_gains(int motor, int p, int i, int d, int pd, int id, int dd);

/*!
 * \ingroup motor
 */
void get_pid_gains(int motor, int *p, int *i, int *d, int *pd, int *id, int *dd);

/*!
 * \ingroup motor
 */
int freeze(int motor);

/*!
 * \ingroup motor
 */
int get_motor_done(int motor);

/*!
 * \ingroup motor
 */
void block_motor_done(int motor);

/*!
 * \see block_motor_done
 * \ingroup motor
 */
void bmd(int motor);

/*!
 * \ingroup motor
 */
int setpwm(int motor, int pwm);

/*!
 * \ingroup motor
 */
int getpwm(int motor);

/*!
 * Moves the given motor forward at full power
 * \param motor the motor's port.
 * \ingroup motor
 */
void fd(int motor);

/*!
 * Moves the given motor backward at full power
 * \param motor the motor's port.
 * \ingroup motor
 */
void bk(int motor);

/*!
 * \ingroup motor
 */
void motor (int motor, int percent);

/*!
 * Turns the specified motor off.
 * \param motor the motor's port.
 * \ingroup motor
 */
void off(int motor);

/*!
 * Turns all motors off.
 * \ingroup motor
 */
void ao();

int get_backemf(int port);

#ifdef __cplusplus
}
#endif


#endif
