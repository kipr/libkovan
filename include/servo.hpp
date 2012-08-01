/*!
 * \file servo.hpp
 * \brief Classes for working with Servos
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _SERVOS_HPP_
#define _SERVOS_HPP_

#include "port.hpp"

/*!
 * \class Servo
 * \brief Encapsulates the concept of a servo motor
 */
class Servo
{
public:
	typedef unsigned int ticks_t;
	
	/*!
	 * Create a new servo object.
	 * \param port The associated physical servo port
	 */
	Servo(port_t port);
	
	/*!
	 * \param position The new servo position, between 0 and 1024
	 *
	 * \note Even though the servos have a _theoretical_ range between 0 and 1024,
	 * the _actual_ range is often less. Setting the servo to a position that it cannot physically
	 * reach will cause the servo to audibly strain and will consume battery very quickly.
	 */
	void setPosition(ticks_t position);
	
	/*!
	 * \return The servo's position as a 10 bit integer
	 * \note Thie method will return the last _sent_ position, not the currently _set_ position.
	 * 
	 * \note For example, imagine the following:
	 * -# myServo.setPosition(700);
	 * -# ... Some time passes ...
	 * -# myServo.setPosition(300);
	 * -# myServo.position();
	 * 
	 * \note position() will return 700 rather than 300,
	 * because 300 hasn't been sent to the servo yet.
	 */
	ticks_t position() const;
private:
	port_t m_port;
};

#endif
