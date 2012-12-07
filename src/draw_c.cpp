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

#include "kovan/draw.h"
#include "kovan/draw.hpp"

Draw *draw_instance()
{
	static Draw s_draw;
	return &s_draw;
}

int draw_open()
{
	return draw_instance()->open();
}

void draw_point(int x, int y)
{
	draw_instance()->point(x, y);
}

void draw_line(int sx, int sy, int ex, int ey)
{
	draw_instance()->line(sx, sy, ex, ey);
}

void draw_clear()
{
	draw_instance()->clear();
}

void draw_close()
{
	draw_instance()->close();
}