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
#include <iostream> // FIXME: tmp
#include <cstdio> // FIXME: tmp
#include "nyi.h"

static const unsigned short motorRegisters[4] = {
	MOTOR_PWM_0,
	MOTOR_PWM_1,
	MOTOR_PWM_2,
	MOTOR_PWM_3,
};

/* static const unsigned short goalRegisters[4] = {
	PID_GOAL_POS_0,
	PID_GOAL_POS_1,
	PID_GOAL_POS_2,
	PID_GOAL_POS_3,
}; */

static const unsigned short bemfRegisters[8] = {
	BEMF_0_HIGH,
	BEMF_0_LOW,
	BEMF_1_HIGH,
	BEMF_1_LOW,
	BEMF_2_HIGH,
	BEMF_2_LOW,
	BEMF_3_HIGH,
	BEMF_3_LOW
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

void Private::Motor::setPidVelocity(const port_t &port, const short &pwm, const bool &hasPos)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	const unsigned short cmd = hasPos ? 0xC000 : 0xA000;
	const unsigned short sign = pwm < 0 ? 0x1000 : 0x0000;
	kovan->enqueueCommand(createWriteCommand(motorRegisters[port], cmd | sign | (pwm & 0x0FFF)));
	std::cout << "Wrote " << std::hex << (cmd | sign | (pwm & 0x0FFF)) << " for pid vel on port " << port << std::endl;
}

short Private::Motor::pidVelocity(const port_t &port) const
{
	return Private::Kovan::instance()->currentState().t[motorRegisters[port]] & 0xFFFF;
}

void Private::Motor::setPidGoalPos(const port_t &port, const short &pos)
{
	// Private::Kovan::instance()->enqueueCommand(createWriteCommand(goalRegisters[port], pos));
}

short Private::Motor::pidGoalPos(const port_t &port) const
{
	// return Private::Kovan::instance()->currentState().t[goalRegisters[port]];
}

void Private::Motor::pid(const port_t &port, short &p, short &i, short &d, short &pd, short &id, short &dd)
{
	nyi("Private::Motor::pid");
}

void Private::Motor::setPwm(const port_t &port, const unsigned char &speed)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	const unsigned int adjustedSpeed = speed > 100 ? 100 : speed; 
	kovan->enqueueCommand(createWriteCommand(motorRegisters[port], adjustedSpeed * 2600 / 100));
}

void Private::Motor::setPwmDirection(const port_t &port, const Motor::Direction &dir)
{
	// FIXME: This assumes that our current state is the latest.
	// If somebody has altered the motor drive codes in the mean time,
	// this will undo their work.
	
	Private::Kovan *kovan = Private::Kovan::instance();
	
	const unsigned short offset = (3 - port) << 1;
	unsigned short &dcs = kovan->currentState().t[MOTOR_DRIVE_CODE_T];
	
	// Clear old drive code
	dcs &= ~(0x3 << offset);
	
	// Add new drive code
	dcs |= dir << offset;
	
#ifdef LIBKOVAN_DEBUG
	std::cout << "PWM Directions: " << std::hex << dcs << std::endl;
#endif
	
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

int Private::Motor::backEMF(const unsigned char &port)
{
	if(port > 3) return 0xFFFF;
	const Private::State &s = Private::Kovan::instance()->currentState();
	return ((int)s.t[bemfRegisters[port << 1]]) << 16 | s.t[bemfRegisters[port << 1 + 1]];
}

Private::Motor *Private::Motor::instance()
{
	static Motor s_motor;
	return &s_motor;
}

Private::Motor::Motor()
{
}
