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

#include "analog_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"
#include "nyi.h"

using namespace Private;

Analog::~Analog()
{
}

void Analog::setPullup(const unsigned char& port, const bool& pullup)
{
	if(port >= 8) return;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &pullups = kovan->currentState().t[AN_PULLUPS];
	
	if(pullup) pullups |= (1 << port);
	else pullups &= ~(1 << port);
	
	kovan->enqueueCommand(createWriteCommand(AN_PULLUPS, pullups));
}

bool Analog::pullup(const unsigned char& port) const
{
	if(port >= 8) return false;
	
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->autoUpdate();
	return kovan->currentState().t[AN_PULLUPS] & (1 << port);
}

unsigned short Analog::value(const unsigned char& port) const
{
	if(port > 16) return 0xFFFF;
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->autoUpdate();
	return kovan->currentState().t[AN_IN_0 + port];
}

bool Analog::isCharging() const
{
	return Private::Kovan::instance()->currentState().t[AC_CONNECTED];
}

Analog *Analog::instance()
{
	static Analog s_analog;
	return &s_analog;
}

Analog::Analog()
{
}

Analog::Analog(const Analog& ) {}
Analog& Analog::operator=(const Analog&) { return *this; }
