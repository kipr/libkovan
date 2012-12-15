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

#include <iostream>

using namespace Private;

Digital::~Digital()
{
}

bool Digital::value(const unsigned char& port) const
{
	const unsigned char actualPort = 7 - (port - 8);
	if(actualPort > 7) return false;
	std::cout << "Digitals: " << std::hex << Private::Kovan::instance()->currentState().t[DIG_IN] << std::endl;
	return Private::Kovan::instance()->currentState().t[DIG_IN] & (1 << actualPort);
}

bool Digital::setValue(const unsigned char& port, const bool& value)
{
	const unsigned char actualPort = 7 - (port - 8);
	if(actualPort > 7) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &out = kovan->currentState().t[DIG_OUT];
	
	if(value) out |= (1 << actualPort);
	else out &= ~(1 << actualPort);
	
	kovan->enqueueCommand(createWriteCommand(DIG_OUT, out));
	wiggle();
	return true;
}

const Digital::Direction Digital::direction(const unsigned char& port) const
{
	const unsigned char actualPort = 7 - (port - 8);
	if(actualPort > 7) return Digital::Unknown;
	Private::Kovan *kovan = Private::Kovan::instance();
	return kovan->currentState().t[DIG_OUT_ENABLE] & (1 << actualPort) ? Digital::Out : Digital::In;
}

bool Digital::setDirection(const unsigned char& port, const Digital::Direction& direction)
{
	const unsigned char actualPort = 7 - (port - 8);
	if(actualPort > 7) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &dir = kovan->currentState().t[DIG_OUT_ENABLE];
	
	if(direction == Digital::Out) dir |= (1 << actualPort);
	else dir &= ~(1 << actualPort);
	
	kovan->enqueueCommand(createWriteCommand(DIG_OUT_ENABLE, dir));
	wiggle();
	return false;
}

bool Digital::pullup(const unsigned char& port) const
{
	const unsigned char actualPort = 7 - (port - 8);
	if(actualPort > 15) return false;
	return Private::Kovan::instance()->currentState().t[DIG_PULLUPS] & (1 << actualPort);
}

bool Digital::setPullup(const unsigned char& port, const bool& pullup)
{
	const unsigned char actualPort = 7 - (port - 8);
	if(actualPort > 7) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &pullups = kovan->currentState().t[DIG_PULLUPS];
	
	if(pullup) pullups |= (1 << actualPort);
	else pullups &= ~(1 << actualPort);
	
	kovan->enqueueCommand(createWriteCommand(DIG_PULLUPS, pullups));
	wiggle();
	return true;
}

Digital *Digital::instance()
{
	static Digital s_digital;
	return &s_digital;
}

void Digital::wiggle()
{
	// TODO: This will be removed in a future version
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->flush();
	kovan->enqueueCommand(createWriteCommand(DIG_UPDATE_T, 1));
	kovan->flush();
	kovan->enqueueCommand(createWriteCommand(DIG_UPDATE_T, 0));
	kovan->flush();
	kovan->enqueueCommand(createWriteCommand(DIG_UPDATE_T, 0));
	kovan->flush();
	kovan->enqueueCommand(createWriteCommand(DIG_UPDATE_T, 1));
	kovan->flush();
}

Digital::Digital()
{
}

Digital::Digital(const Digital&) {}
Digital& Digital::operator=(const Digital&) { return *this; }
