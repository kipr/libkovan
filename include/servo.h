/*!
 * \file servo.h
 * \brief Methods for working with Servos
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _SERVOS_H_
#define _SERVOS_H_

#ifdef __cplusplus
extern "C" {
#endif

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
 */
int get_servo_position(int servo);

/*!
 * \param servo The port of the servo
 * \param position The new servo position, between 0 and 1024
 *
 * \note Even though the servos have a _theoretical_ range between 0 and 1024,
 * the _actual_ range is often less. Setting the servo to a position that it cannot physically
 * reach will cause the servo to audibly strain and will consume battery very quickly.
 */
void set_servo_position(int servo, int position);

#ifdef __cplusplus
}
#endif


#endif
