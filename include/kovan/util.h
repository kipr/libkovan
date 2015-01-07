/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

/*!
 * \file util.h
 * \brief Utility methods
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup util Utilties
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include "export.h"
#include "vtable.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Sleeps for a given number of milliseconds.
 * \param msecs The number of milliseconds to sleep.
 * \blocks
 * \ingroup util
 */
VF EXPORT_SYM void msleep(long msecs);

/*!
 * Get the current system time in milliseconds.
 * \return the number of milliseconds since the UNIX epoch (January 1st, 1970)
 * \ingroup util
 */
VF EXPORT_SYM unsigned long systime();

/*!
 * Get the current system time in seconds.
 * \return the number of seconds since the UNIX epoch (January 1st, 1970)
 * \ingroup util
 */
VF EXPORT_SYM double seconds();

VFL

#ifdef __cplusplus
}
#endif

#endif
