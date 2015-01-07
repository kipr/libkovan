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

VH #include "kovan/digital.h"
#include "digital_p.hpp"

VI int digital(int port)
{
	return !get_digital_value(port);
}

VI void set_digital_value(int port, int value)
{
	Private::Digital::instance()->setValue(port, value);
}

VI int get_digital_value(int port)
{
	return Private::Digital::instance()->value(port);
}

VI void set_digital_output(int port, int out)
{
	Private::Digital::instance()->setDirection(port, out ? Private::Digital::Out : Private::Digital::In);
}

VI int get_digital_output(int port)
{
	return Private::Digital::instance()->direction(port) == Private::Digital::Out;
}

VI int get_digital_pullup(int port)
{
	return Private::Digital::instance()->pullup(port);
}

VI void set_digital_pullup(int port, int pullup)
{
	Private::Digital::instance()->setPullup(port, pullup);
}