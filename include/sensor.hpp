/*!
 * \file sensor.hpp
 * \brief Base sensor classes
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup sensor Sensors
 */

#ifndef _SENSORS_HPP_
#define _SENSORS_HPP_

/*!
 * \class Sensor
 * \brief The base class for all sensors of any type
 * \tparam T The return type of this sensor.
 * For example, a sensor returning true or false should be of type bool.
 * \ingroup sensor
 */
template<typename T>
class Sensor
{
public:
	virtual ~Sensor() {}
	/*!
	 * Get the current value for this sensor
	 * \return The sensor's current value
	 */
	virtual T value() const = 0;
};

#endif
