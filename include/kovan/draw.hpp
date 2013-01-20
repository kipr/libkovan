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

/*!
 * \file draw.hpp
 * \brief Classes for drawing to the screen
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup draw Drawing
 */

#ifndef _DRAW_HPP_
#define _DRAW_HPP_

 #include "export.h"

namespace Private
{
	class Draw;
}

class EXPORT_SYM Draw
{
public:
	Draw();
	~Draw();
	
	bool open();
	void point(int x, int y);
	void line(int sx, int sy, int ex, int ey);
	void clear();
	void raise();
	void setSize(const unsigned int& width, const unsigned int& height);
	void close();
	
private:
	Private::Draw *p_draw;
};

#endif
