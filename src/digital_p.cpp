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

#include "digital_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"
#include "nyi.h"

using namespace Private;

Digital::~Digital()
{
}

bool Digital::value(const unsigned char& port) const
{
	if(port > 7) return false;
	return Private::Kovan::instance()->currentState().t[DIG_IN] & (0x1 << port);
}

bool Digital::setValue(const unsigned char& port, const bool& value)
{
	nyi("Private::Digital::setValue");
	return false;
}

const Digital::Direction Digital::direction(const unsigned char& port) const
{
	nyi("Private::Digital::direction");
	return Digital::Unknown;
}

bool Digital::setDirection(const unsigned char& port, const Digital::Direction& direction)
{
	nyi("Private::Digital::setDirection");
	return false;
}

bool Digital::pullup(const unsigned char& port) const
{
	nyi("Private::Digital::pullup");
	return false;
}

bool Digital::setPullup(const unsigned char& port, const bool& pullup)
{
	nyi("Private::Digital::setPullup");
	return false;
}

Digital *Digital::instance()
{
	static Digital s_digital;
	return &s_digital;
}

Digital::Digital()
{
}

Digital::Digital(const Digital&) {}
Digital& Digital::operator=(const Digital&) { return *this; }