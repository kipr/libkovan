/*!
 * \file digital.hpp
 * \brief Classes for working with digital sensors
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \ingroup sensor
 */

#ifndef _DIGITAL_HPP_
#define _DIGITAL_HPP_

#include "sensor.hpp"

/*!
 * \class Digital
 * \brief Facilitates access to a digital sensor
 * \author Braden McDorman
 * \ingroup sensor
 */
class Digital : public Sensor<bool>
{
public:
	Digital(const unsigned char& port);
	
	virtual bool setValue(const bool& value);
	
	/*!
	 * Gets the current value of the digital sensor
	 */
	virtual bool value() const;
	
	virtual bool setOutput(const bool& output);
	virtual bool isOutput() const;
	
	virtual void setPullup(const bool& pullup);
	virtual bool pullup() const;
private:
	unsigned char m_port;
};

#endif
