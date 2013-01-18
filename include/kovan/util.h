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
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Sleeps for a given number of milliseconds.
 * \param msecs The number of milliseconds to sleep.
 * \blocks
 */
EXPORT_SYM void msleep(long msecs);

EXPORT_SYM unsigned long systime();
EXPORT_SYM double seconds();

#ifdef __cplusplus
}
#endif

#endif
