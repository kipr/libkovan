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

#include "sensor_logic.hpp"

using namespace SensorLogic;

Base::Base(const Sensor<bool> *a, const Sensor<bool> *b) : m_a(a), m_b(b) {}

const Sensor<bool> *Base::a() const
{
	return m_a;
}

const Sensor<bool> *Base::b() const
{
	return m_b;
}

And::And(const Sensor<bool> *a, const Sensor<bool> *b) : Base(a, b) {}

bool And::value() const
{
	return a()->value() && b()->value();
}

Or::Or(const Sensor<bool> *a, const Sensor<bool> *b) : Base(a, b) {}

bool Or::value() const
{
	return a()->value() || b()->value();
}

Xor::Xor(const Sensor<bool> *a, const Sensor<bool> *b) : Base(a, b) {}

bool Xor::value() const
{
	const bool b = Base::b()->value();
	return a()->value() ? !b : b;
}