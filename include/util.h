/*!
 * \file util.h
 * \brief Utility methods
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Sleeps for a given number of milliseconds.
 * \param msecs The number of milliseconds to sleep.
 * \blocks
 */
void msleep(long msecs);

#ifdef __cplusplus
}
#endif

#endif
