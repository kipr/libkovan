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

#include "kovan/analog.h"
#include "analog_p.hpp"

int analog10(int port)
{
	return Private::Analog::instance()->value(static_cast<unsigned char>(port));
}

int analog(int port)
{
	return analog10(port) >> 2;
}

void set_analog_pullup(int port, int pullup)
{
	Private::Analog::instance()->setPullup(static_cast<unsigned char>(port), pullup == 0 ? false : true);
}

int get_analog_pullup(int port)
{
	return Private::Analog::instance()->pullup(static_cast<unsigned char>(port));
}