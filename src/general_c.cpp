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

VH #include "kovan/general.h"
#include "kovan_p.hpp"

VH #include "kovan/motors.h"
VH #include "kovan/servo.h"
VH #include "kovan/analog.h"
VH #include "kovan/create.h"
VH #include "kovan/digital.h"

VI void set_auto_publish(int on)
{
	Private::Kovan::instance()->setAutoFlush(on);
}

VI void publish()
{
	Private::Kovan::instance()->flush();
}

VI void halt()
{
	freeze_halt();
	disable_servos();

}

void freeze_halt()
{
	ao();
	for(int i = 0; i < 8; ++i) {
		set_analog_pullup(i, 1);
	}
	for(int i = 8; i < 16; ++i) {
		set_digital_output(i, 0);
		set_digital_pullup(i, 1);
	}
	create_stop();
}