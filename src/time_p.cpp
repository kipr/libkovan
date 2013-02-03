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

#include "time_p.hpp"

#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <algorithm>
#include <stdio.h>

void Private::Time::microsleep(unsigned long microsecs)
{
	// So... it turns out usleep isn't defined for values larger than 1000000.
	// This is the workaround.
	while(microsecs) {
		const unsigned long current = std::min(microsecs, 999999UL);
		if(usleep(current)) {
			perror("usleep");
			return;
		}
		microsecs -= current;
	}
}

unsigned long Private::Time::systime()
{
	timeval t;
	gettimeofday(&t, 0);
	return ((unsigned long)t.tv_sec) * 1000L + t.tv_usec / 1000L;
}
