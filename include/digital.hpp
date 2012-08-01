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
	
	virtual bool value() const;
private:
	unsigned char m_port;
};

#endif
