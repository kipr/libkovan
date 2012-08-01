/*!
 * \file battery.h
 * \brief Methods for determining the device's power level
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _BATTERY_H_
#define _BATTERY_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * The device's power level
 * \return The device's current voltage
 */
float power_level();

#ifdef __cplusplus
}
#endif

#endif
