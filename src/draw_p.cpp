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

#if BUILD_WITH_QT

#include "draw_p.hpp"
#include <QGraphicsScene>
#include <QCoreApplication>
#include <QApplication>

bool Private::Draw::s_inited = false;

Private::Draw::Draw()
	: m_scene(new QGraphicsScene(this))
{
	setScene(m_scene);
}

Private::Draw::~Draw()
{
	delete m_scene;
}

bool Private::Draw::open()
{
	show();
	raise();
	return true;
}

void Private::Draw::point(const int& x, const int& y)
{
	line(x, y, x, y);
}

void Private::Draw::line(const int& sx, const int& sy, const int& ex, const int& ey)
{
	m_scene->addLine(sx, sy, ex, ey);
}

void Private::Draw::clear()
{
	m_scene->clear();
}

void Private::Draw::setSize(const unsigned int& width, const unsigned int& height)
{
	resize(width, height);
}

void Private::Draw::raise()
{
	QGraphicsView::raise();
}

void Private::Draw::close()
{
	hide();
}

bool Private::Draw::isInited()
{
	return s_inited;
}

void Private::Draw::init()
{
	if(isInited()) return;
	static char *argv[] = {"beta"};
	static int argc = 1;
	QApplication *app = new QApplication(argc, argv);
	s_inited = true;
}

#endif