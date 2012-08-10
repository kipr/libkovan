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

#ifndef _DRAW_P_X11_HPP_
#define _DRAW_P_X11_HPP_

#include "build_options.h"

#if BUILD_WITH_X11

#include <cairo/cairo.h>
#include <cairo/cairo-pdf.h>
#include <cairo/cairo-ps.h>
#include <cairo/cairo-xlib.h>
#include <X11/Xlib.h>

namespace Private
{
	class Draw 
	{
	public:
		Draw();
		~Draw();
		
		bool open();
		void point(const int& x, const int& y);
		void line(const int& sx, const int& sy, const int& ex, const int& ey);
		void clear();
		void setSize(const unsigned int& width, const unsigned int& height);
		void raise();
		void close();
		
		bool isInited();
		void init();
		
		void update();
	private:
		bool m_inited;
		void paint();
		
		Display *m_display;
		Window m_rootWindow;
		Window m_window;
		Colormap m_colorMap;
		GC m_gc;
		int m_screen;
		cairo_surface_t *m_surface;
	};
}

#endif

#endif
