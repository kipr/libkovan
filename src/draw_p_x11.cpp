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

#include "build_options.h"

#if BUILD_WITH_X11

#include "draw_p_x11.hpp"

#define DEFAULT_WIDTH 320
#define DEFAULT_HEIGHT 240

Private::Draw::Draw()
	: m_inited(false)
{

}

Private::Draw::~Draw()
{
	if(!isInited()) return;
	cairo_surface_destroy(m_surface);
	XCloseDisplay(m_display);
}

bool Private::Draw::open()
{
	m_window = XCreateSimpleWindow(m_display, m_rootWindow, 1, 1, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0,
		BlackPixel(m_display, m_screen), BlackPixel(m_display, m_screen));

	XStoreName(m_display, m_window, "hello");
	XSelectInput(m_display, m_window, ExposureMask | ButtonPressMask);
	XMapWindow(m_display, m_window);
	
	m_surface = cairo_xlib_surface_create(m_display, m_window, DefaultVisual(m_display, 0), DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
	return true;
}

void Private::Draw::point(const int& x, const int& y)
{
	
}

void Private::Draw::line(const int& sx, const int& sy, const int& ex, const int& ey)
{
	
}

void Private::Draw::clear()
{

}

void Private::Draw::setSize(const unsigned int& width, const unsigned int& height)
{
	XResizeWindow(m_display, m_window, width, height);
}

void Private::Draw::raise()
{
	
}

void Private::Draw::close()
{
	XDestroyWindow(m_display, m_window);
}

void Private::Draw::update()
{
	XEvent e;
	XNextEvent(m_display, &e);
	if(e.type == Expose && e.xexpose.count < 1) paint();
	
}

void Private::Draw::paint()
{
	cairo_t *context = cairo_create(m_surface);
	cairo_rectangle(context, 0.0, 0.0, DEFAULT_HEIGHT, DEFAULT_WIDTH);
	cairo_set_source_rgb(context, 0.0, 0.0, 0.5);
	cairo_fill(context);
	cairo_move_to(context, 10.0, 10.0);
	cairo_set_source_rgb(context, 1.0, 1.0, 0.0);
	cairo_show_text(context, "Hello World!");
	cairo_show_page(context);
	cairo_destroy(context);
}

bool Private::Draw::isInited()
{
	return m_inited;
}

void Private::Draw::init()
{
	if(isInited()) return;
	
	if(!(m_display = XOpenDisplay(NULL))) {
		fprintf(stderr, "ERROR: Could not open display\n");
		return false;
	}

	m_screen = DefaultScreen(m_display);
	m_rootWindow = RootWindow(m_display, m_screen);
	
	m_inited = true;
}

#endif