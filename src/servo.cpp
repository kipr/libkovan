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

#include "kovan/servo.hpp"
#include "servo_p.hpp"

Servo::Servo(port_t port)
	: m_port(port)
{
}

void Servo::setPosition(Servo::ticks_t position)
{
	Private::Servo::instance()->setPosition(m_port, position);
}

Servo::ticks_t Servo::position() const
{
	return Private::Servo::instance()->position(m_port);
}

void Servo::disable()
{
	setEnabled(false);
}

void Servo::enable()
{
	setEnabled(true);
}

void Servo::setEnabled(const bool &enabled)
{
	Private::Servo::instance()->setEnabled(m_port, enabled);
}

bool Servo::isEnabled() const
{
	return Private::Servo::instance()->isEnabled(m_port);
}