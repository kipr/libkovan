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

#include "kovan/motors.hpp"
#include "kovan/util.h"
#include "motors_p.hpp"
#include <cstdlib>
#include <math.h>

Motor::Motor(const port_t& port) throw()
	: m_port(port)
{
	// if(!p_motor) throw InvalidPort("Motor ports are through 1 and 4");
}

void Motor::clearPositionCounter()
{
	Private::Motor::instance()->clearBemf(m_port);
}

void Motor::moveAtVelocity(const short& velocity)
{
	Private::Motor::instance()->setControlMode(m_port, Private::Motor::Speed);
	Private::Motor::instance()->setPidVelocity(m_port, velocity);
}

void Motor::moveToPosition(const short& speed, const int& goalPos)
{
	short velocity = std::abs(speed);
	const int sign = Private::Motor::instance()->backEMF(m_port) > goalPos ? 1 : -1;
	velocity *= sign;
	Private::Motor::instance()->setControlMode(m_port, Private::Motor::SpeedPosition);
	Private::Motor::instance()->setPidGoalPos(m_port, goalPos);
	Private::Motor::instance()->setPidVelocity(m_port, velocity);
}

void Motor::moveRelativePosition(const short& speed, const int& deltaPos)
{
	moveToPosition(speed, Private::Motor::instance()->backEMF(m_port) + deltaPos);
}

void Motor::setPidGains(const short& p, const short& i, const short& d, const short& pd, const short& id, const short& dd)
{
	Private::Motor::instance()->setPidGains(m_port, p, i, d, pd, id, dd);
}

void Motor::pidGains(short& p, short& i, short& d, short& pd, short& id, short& dd)
{
	Private::Motor::instance()->pidGains(m_port,p, i, d, pd, id, dd);
}

void Motor::freeze()
{
	Private::Motor::instance()->setPwm(m_port, 100);
	Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::ActiveStop);
}

bool Motor::isMotorDone() const
{
	return !Private::Motor::instance()->isPidActive(m_port);
}

void Motor::blockMotorDone() const
{
	msleep(50);
	while(isMotorDone()) msleep(40);
}

void Motor::forward()
{
	motor(100);
}

void Motor::backward()
{
	motor(-100);
}

void Motor::motor(int percent)
{
	Private::Motor::instance()->setControlMode(m_port, Private::Motor::Inactive);
	Private::Motor::instance()->setPwm(m_port, std::abs(percent));
	if(percent > 0) Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::Forward);
	else if(percent < 0) Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::Reverse);
	else Private::Motor::instance()->setPwmDirection(m_port, Private::Motor::PassiveStop);
}

void Motor::off()
{
	Private::Motor::instance()->stop(m_port);
}

const port_t& Motor::port() const
{
	return m_port;
}

BackEMF::BackEMF(const unsigned char& port)
	: m_port(port)
{}

int BackEMF::value() const
{
	return Private::Motor::instance()->backEMF(m_port);
}

unsigned char BackEMF::port() const
{
	return m_port;
}
