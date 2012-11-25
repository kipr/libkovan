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
	// FIXME: Untested
	if(port < 8 || port > 15) return false;
	return Private::Kovan::instance()->currentState().t[DIG_IN] & (1 << (port - 8));
}

bool Digital::setValue(const unsigned char& port, const bool& value)
{
	// FIXME: Untested
	if(port < 8 || port > 15) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &out = kovan->currentState().t[DIG_OUT];
	
	if(value) out |= (1 << (port - 8));
	else out &= ~(1 << (port - 8));
	
	kovan->enqueueCommand(createWriteCommand(DIG_OUT, out));
	wiggle();
	return true;
}

const Digital::Direction Digital::direction(const unsigned char& port) const
{
	// FIXME: Untested
	if(port < 8 || port > 15) return Digital::Unknown;
	Private::Kovan *kovan = Private::Kovan::instance();
	return kovan->currentState().t[DIG_OUT_ENABLE] & (1 << (port - 8)) ? Digital::Out : Digital::In;
}

bool Digital::setDirection(const unsigned char& port, const Digital::Direction& direction)
{
	// FIXME: Untested
	if(port < 8 || port > 15) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &pullups = kovan->currentState().t[DIG_OUT_ENABLE];
	
	if(direction == Digital::Out) pullups |= (1 << (port - 8));
	else pullups &= ~(1 << (port - 8));
	
	kovan->enqueueCommand(createWriteCommand(DIG_OUT_ENABLE, pullups));
	wiggle();
	return false;
}

bool Digital::pullup(const unsigned char& port) const
{
	// FIXME: Untested
	if(port < 8 || port > 15) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	return kovan->currentState().t[DIG_PULLUPS] & (1 << (port - 8));
}

bool Digital::setPullup(const unsigned char& port, const bool& pullup)
{
	// FIXME: Untested
	if(port < 8 || port > 15) return false;
	Private::Kovan *kovan = Private::Kovan::instance();
	unsigned short &pullups = kovan->currentState().t[DIG_PULLUPS];
	
	if(pullup) pullups |= (1 << (port - 8));
	else pullups &= ~(1 << (port - 8));
	
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
