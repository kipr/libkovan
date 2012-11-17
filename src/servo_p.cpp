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

#include "servo_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"
#include "private.hpp"
#include <cstdio>

static const unsigned short servoRegisters[4] = {
	SERVO_COMMAND_0,
	SERVO_COMMAND_1,
	SERVO_COMMAND_2,
	SERVO_COMMAND_3
};

Private::Servo::Servo()
{
}

Private::Servo::~Servo()
{
}

bool Private::Servo::setPosition(const port_t& port, const unsigned short& position)
{
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(servoRegisters[port], position));
	
	return true; // TODO: Remove return value?
}

unsigned short Private::Servo::position(const port_t& port) const
{
	return Private::Kovan::instance()->currentState().t[servoRegisters[port]];
}

Private::Servo *Private::Servo::instance()
{
	static Servo instance;
	return &instance;
}