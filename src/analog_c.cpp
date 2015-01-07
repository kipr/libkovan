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

VH #include "kovan/analog.h"
#include "analog_p.hpp"

VI int analog(int port)
{
  return analog10(port);
}

VI int analog10(int port)
{
  set_analog_pullup(port, 1);
  return Private::Analog::instance()->value(static_cast<unsigned char>(port));
}

VI int analog_et(int port)
{
  set_analog_pullup(port, 0);
  return Private::Analog::instance()->value(static_cast<unsigned char>(port));
}

VI int analog8(int port)
{
	return analog(port) >> 2;
}

VI void set_analog_pullup(int port, int pullup)
{
  Private::Analog::instance()->setPullup(static_cast<unsigned char>(port),
    pullup == 0 ? false : true);
}

VI int get_analog_pullup(int port)
{
	return Private::Analog::instance()->pullup(static_cast<unsigned char>(port));
}