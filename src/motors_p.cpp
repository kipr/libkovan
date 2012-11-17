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

#include "motors_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"
#include <iostream> // TMP
#include <cstdio> // TMP
#include "nyi.h"

static const short motorRegisters[4] = {
	MOTOR_PWM_0,
	MOTOR_PWM_1,
	MOTOR_PWM_2,
	MOTOR_PWM_3,
};

Private::Motor::~Motor()
{
}

void Private::Motor::setControlMode(const Motor::ControlMode &mode)
{
	nyi("Private::Motor::setControlMode");
}

Private::Motor::ControlMode Private::Motor::controlMode() const
{
	nyi("Private::Motor::controlMode");
	return PWM;
}

void Private::Motor::setPid(const port_t &port, const short &p, const short &i, const short &d, const short &pd, const short &id, const short &dd)
{
	nyi("Private::Motor::setPid");
}

void Private::Motor::pid(const port_t &port, short &p, short &i, short &d, short &pd, short &id, short &dd)
{
	nyi("Private::Motor::pid");
}

void Private::Motor::setPwm(const port_t &port, const unsigned char &speed)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->enqueueCommand(createWriteCommand(motorRegisters[port], speed));
}

void Private::Motor::setPwmDirection(const port_t &port, const Motor::Direction &dir)
{
	// FIXME: This assumes that our current state is the latest.
	// If somebody has altered the motor drive codes in the mean time,
	// this will undo their work.
	
	Private::Kovan *kovan = Private::Kovan::instance();
	
	const unsigned short offset = port << 1;
	unsigned short dcs = kovan->currentState().t[MOTOR_DRIVE_CODE_T];
	
	// Clear old drive code
	dcs &= ~(0x3 << offset);
	
	// Add new drive code
	dcs |= dir << offset;
	
	kovan->enqueueCommand(createWriteCommand(MOTOR_DRIVE_CODE_T, dcs));
}

unsigned char Private::Motor::pwm(const port_t &port)
{
	return Private::Kovan::instance()->currentState().t[motorRegisters[port]];
}

void Private::Motor::stop(const port_t &port)
{
	setPwmDirection(port, PassiveStop);
}

unsigned short Private::Motor::backEMF(const unsigned char &port)
{
	nyi("Private::Motor::backEMF");
	return 0xFFFF;
}

Private::Motor *Private::Motor::instance()
{
	static Motor s_motor;
	return &s_motor;
}

Private::Motor::Motor()
{
}
