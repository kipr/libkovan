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

#include "kovan/analog.hpp"
#include "analog_p.hpp"

Analog::Analog(const unsigned char& port)
	: m_port(port)
{
	
}

unsigned short Analog::value() const
{
	return Private::Analog::instance()->value(m_port);
}

void Analog::setPullup(bool pullup)
{
	return Private::Analog::instance()->setPullup(m_port, pullup);
}

bool Analog::pullup() const
{
	return Private::Analog::instance()->pullup(m_port);
}

Analog8::Analog8(const unsigned char& port) : Analog(port) {}

unsigned short Analog8::value() const
{
	return Analog::value() >> 2;
}