/*!
 * \file analog.hpp
 * \brief Classes for working with analog sensors
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \ingroup sensor
 */

#ifndef _ANALOG_HPP_
#define _ANALOG_HPP_

#include "sensor.hpp"

/*!
 * \class Analog
 * \brief Implements the sensor type "analog"
 * \details Returns 10 bit unsigned integers
 * \see Analog8
 * \author Braden McDorman
 * \ingroup sensor
 */
class Analog : public Sensor<unsigned short>
{
public:
	Analog(const unsigned char& port);
	
	virtual unsigned short value() const;
	
	virtual void setPullup(bool pullup);
	virtual bool pullup() const;
	
	/*!
	 * Gets the physical port associated with this analog instance
	 * \return physical port address
	 */
	unsigned char port() const;
	
private:
	unsigned char m_port;
};

/*!
 * \class Analog8
 * \brief A derivative of the Analog class that returns 8 bit values rather than 10 bit ones.
 * \details Returns 8 bit unsigned integers
 * \see Analog
 * \author Braden McDorman
 * \ingroup sensor
 */
class Analog8 : public Analog
{
public:
	Analog8(const unsigned char& port);
	virtual unsigned short value() const;
};

#endif
