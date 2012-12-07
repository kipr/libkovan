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
 * \file create_codes.h
 * \author Braden McDorman
 * \copyright KISS Institute for Practical Robotics
 * \ingroup create
 */

#ifndef _CREATE_CODES_H_
#define _CREATE_CODES_H_

// See http://www.irobot.com/filelibrary/pdfs/hrd/create/create%20open%20interface_v2.pdf
// for more information

#define OI_START 128
#define OI_BAUD 129
#define OI_SAFE 131
#define OI_FULL 132
#define OI_DEMO 136

#define OI_DRIVE 137
#define OI_DRIVE_DIRECT 145
#define OI_LEDS 139
#define OI_DIGITAL_OUTPUTS 147
#define OI_LOW_SIDE_DRIVERS 138
#define OI_SEND_IR 151

#define OI_SONG 140
#define OI_PLAY_SONG 141

#define OI_SENSORS 142
#define OI_QUERY_LIST 149

#define OI_STREAM 148
#define OI_PAUSE_RESUME_STREAM 150

#define OI_SCRIPT 152
#define OI_PLAY_SCRIPT 153
#define OI_SHOW_SCRIPT 154

#define OI_WAIT_TIME 155
#define OI_WAIT_DISTANCE 156
#define OI_WAIT_ANGLE 157
#define OI_WAIT_EVENT 156

#define OI_BUMPS_AND_WHEEL_DROPS 7
#define OI_WALL 8
#define OI_CLIFF_LEFT 9
#define OI_CLIFF_FRONT_LEFT 10
#define OI_CLIFF_FRONT_RIGHT 11
#define OI_CLIFF_RIGHT 12
#define OI_VIRTUAL_WALL 13
#define OI_LOW_SIDE_DRIVER_AND_WHEEL_OVERCURRENTS 14
#define OI_BUTTONS 18
#define OI_DISTANCE 19
#define OI_ANGLE 20
#define OI_CHARGING_STATE 21
#define OI_VOLTAGE 22
#define OI_CURRENT 23
#define OI_CARGO_BAY_DIGITAL_INPUTS 32


#endif
