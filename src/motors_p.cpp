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

static const unsigned short goalPosLowRegisters[4] = {
	GOAL_POS_0_LOW,
	GOAL_POS_1_LOW,
	GOAL_POS_2_LOW,
	GOAL_POS_3_LOW
};

static const unsigned short goalPosHighRegisters[4] = {
	GOAL_POS_0_HIGH,
	GOAL_POS_1_HIGH,
	GOAL_POS_2_HIGH,
	GOAL_POS_3_HIGH
};

static const unsigned short goalSpeedLowRegisters[4] = {
	GOAL_SPEED_0_LOW,
	GOAL_SPEED_1_LOW,
	GOAL_SPEED_2_LOW,
	GOAL_SPEED_3_LOW
};

static const unsigned short goalSpeedHighRegisters[4] = {
	GOAL_SPEED_0_HIGH,
	GOAL_SPEED_1_HIGH,
	GOAL_SPEED_2_HIGH,
	GOAL_SPEED_3_HIGH
};

static const unsigned short bemfLowRegisters[8] = {
	BEMF_0_LOW,
	BEMF_1_LOW,
	BEMF_2_LOW,
	BEMF_3_LOW
};

static const unsigned short bemfHighRegisters[8] = {
	BEMF_0_HIGH,
	BEMF_1_HIGH,
	BEMF_2_HIGH,
	BEMF_3_HIGH,
};

Private::Motor::~Motor()
{
}


void Private::Motor::setPidGains(port_t port, const short &p, const short &i, const short &d, const short &pd, const short &id, const short &dd)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->enqueueCommand(createWriteCommand(PID_P_0 + port, p));
	kovan->enqueueCommand(createWriteCommand(PID_I_0 + port, i));
	kovan->enqueueCommand(createWriteCommand(PID_D_0 + port, d));
	kovan->enqueueCommand(createWriteCommand(PID_PD_0 + port, pd));
	kovan->enqueueCommand(createWriteCommand(PID_ID_0 + port, id));
	kovan->enqueueCommand(createWriteCommand(PID_DD_0 + port, dd));
}

void Private::Motor::clearBemf(unsigned char port)
{
	if(port > 3) return;
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(MOT_BEMF_CLEAR, 1 << port));
}

void Private::Motor::setControlMode(port_t port, Private::Motor::ControlMode controlMode)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	
	const unsigned short offset = (3 - port) << 1;
	unsigned short &modes = kovan->currentState().t[PID_MODES];
	
	// Clear old drive code
	modes &= ~(0x3 << offset);
	// Add new drive code
	modes |= ((int)(controlMode)) << offset;
	
	kovan->enqueueCommand(createWriteCommand(PID_MODES, modes));
}

Private::Motor::ControlMode Private::Motor::controlMode(port_t port) const
{
	Private::Kovan *kovan = Private::Kovan::instance();
	
	const unsigned short offset = (3 - port) << 1;
	unsigned short &modes = kovan->currentState().t[PID_MODES];
	
	// Clear old drive code
	modes &= ~(0x3 << offset);
	
	return (Private::Motor::ControlMode)((modes >> offset) & 0x3);
}

bool Private::Motor::isPidActive(port_t port) const
{
	Private::Kovan *kovan = Private::Kovan::instance();
	return kovan->currentState().t[PID_STATUS];
}

void Private::Motor::setPidVelocity(const port_t &port, const int &ticks)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->enqueueCommand(createWriteCommand(goalSpeedLowRegisters[port], ticks & 0x0000FFFF));
	kovan->enqueueCommand(createWriteCommand(goalSpeedHighRegisters[port], (ticks & 0xFFFF0000) >> 16));
}

int Private::Motor::pidVelocity(const port_t &port) const
{
	const State &state = Private::Kovan::instance()->currentState();
	return state.t[goalSpeedHighRegisters[port]] << 16 || state.t[goalSpeedLowRegisters[port]];
}

void Private::Motor::setPidGoalPos(const port_t &port, const int &pos)
{
	Private::Kovan *kovan = Private::Kovan::instance();
	kovan->enqueueCommand(createWriteCommand(goalPosLowRegisters[port], pos & 0x0000FFFF));
	kovan->enqueueCommand(createWriteCommand(goalPosHighRegisters[port], (pos & 0xFFFF0000) >> 16));
}

int Private::Motor::pidGoalPos(const port_t &port) const
{
	const State &state = Private::Kovan::instance()->currentState();
	return state.t[goalPosHighRegisters[port]] << 16 || state.t[goalPosLowRegisters[port]];
}

void Private::Motor::pidGains(port_t port, short &p, short &i, short &d, short &pd, short &id, short &dd)
{
	if(port > 3) return;
	const State &state = Private::Kovan::instance()->currentState();
	p = state.t[PID_P_0 + port];
	i = state.t[PID_I_0 + port];
	d = state.t[PID_D_0 + port];
	pd = state.t[PID_PD_0 + port];
	id = state.t[PID_ID_0 + port];
	dd = state.t[PID_DD_0 + port];
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
	return ((int)s.t[bemfHighRegisters[port]]) << 16 | s.t[bemfLowRegisters[port]];
}

Private::Motor *Private::Motor::instance()
{
	static Motor s_motor;
	return &s_motor;
}

Private::Motor::Motor()
{
}
