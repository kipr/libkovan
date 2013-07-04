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
#include <cstdio>

static const unsigned short servoRegisters[4] = {
	SERVO_COMMAND_0,
	SERVO_COMMAND_1,
	SERVO_COMMAND_2,
	SERVO_COMMAND_3
};

#define TIMEDIV (1.0 / 13000000) // 13 MHz clock
#define PWM_PERIOD_RAW 0.02F
#define SERVO_MAX_RAW 0.0025f
#define SERVO_MIN_RAW 0.0005f
#define PWM_PERIOD ((unsigned int)(PWM_PERIOD_RAW / TIMEDIV))
#define SERVO_MAX (SERVO_MAX_RAW / TIMEDIV)
#define SERVO_MIN (SERVO_MIN_RAW / TIMEDIV)

Private::Servo::Servo()
{
}

Private::Servo::~Servo()
{
}

void Private::Servo::setEnabled(port_t port, const bool &enabled)
{
	port = fixPort(port);
	if(port > 3) return;
	unsigned short &allStop = Private::Kovan::instance()->currentState().t[MOTOR_ALL_STOP];
	const unsigned short val = 1 << (port + 1);
	if(enabled) allStop |= val;
	else allStop &= ~val;
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(MOTOR_ALL_STOP, allStop));
}

bool Private::Servo::isEnabled(port_t port)
{
	port = fixPort(port);
	if(port > 3) return false;
	unsigned short &allStop = Private::Kovan::instance()->currentState().t[MOTOR_ALL_STOP];
	const unsigned short val = 1 << (port + 1);
	return allStop & val;
}

bool Private::Servo::setPosition(port_t port, const unsigned short& position)
{
	port = fixPort(port);
	if(port > 3) return false;
	unsigned short cappedPosition = position & 0x07FF;
	const unsigned short val = 6500 + ((cappedPosition*26000) / 2047);
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(servoRegisters[port], val));
	return true; // TODO: Remove return value?
}

unsigned short Private::Servo::position(port_t port) const
{
	port = fixPort(port);
	if(port > 3) return 0xFFFF;
	const unsigned int val = Private::Kovan::instance()->currentState().t[servoRegisters[port]];
	return 1 + ((2047 * (val - 6500)) / 26000);
}

Private::Servo *Private::Servo::instance()
{
	static Servo instance;
	return &instance;
}

port_t Private::Servo::fixPort(port_t port) const
{
	switch(port) {
		case 0: return 1;
		case 1: return 0;
		case 2: return 3;
		case 3: return 2;
	}
	return port;
}
