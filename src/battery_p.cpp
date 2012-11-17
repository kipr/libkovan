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

#include "battery_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"
#include "nyi.h"
#include "private.hpp"

#include <cstdio>

Private::Battery::level_t Private::Battery::level()
{
	nyi("Private::Battery::level");
	return 0;
}

Private::Battery *Private::Battery::instance()
{
	static Battery s_instance;
	return &s_instance;
}

Private::Battery::Battery() {}