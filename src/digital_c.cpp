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

#include "digital.h"
#include "digital_p.hpp"

int digital(int port)
{
	return get_digital_value();
}

void set_digital_value(int port, int value)
{
	Private::Digital::instance()->setValue(m_port, value);
}

int get_digital_value(int port)
{
	return Private::Digital::instance()->value(m_port);
}

void set_digital_output(int port, int out)
{
	Private::Digital::instance()->setDirection(m_port, output ? Private::Digital::Out : Private::Digital::In);
}

int get_digital_output(int port)
{
	return Private::Digital::instance()->direction(m_port) == Private::Digital::Out;
}

int get_digital_pullup(int port)
{
	return Private::Digital::instance()->pullup(m_port);
}

void set_digital_pullup(int port, int pullup)
{
	Private::Digital::instance()->setPullup(m_port, pullup);
}