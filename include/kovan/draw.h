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

#ifndef _DRAW_H_
#define _DRAW_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

EXPORT_SYM int draw_open();
EXPORT_SYM void draw_point(int x, int y);
EXPORT_SYM void draw_line(int sx, int sy, int ex, int ey);
EXPORT_SYM void draw_clear();
EXPORT_SYM void draw_close();

#ifdef __cplusplus
}
#endif


#endif
