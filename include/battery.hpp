/*!
 * \file battery.hpp
 * \brief Classes for determining the device's power level
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _BATTERY_HPP_
#define _BATTERY_HPP_

/*!
 * \class Battery
 * \brief Provides helper methods for accessing the device's power level
 */
class Battery
{
public:
	static float powerLevel();
};

#endif
