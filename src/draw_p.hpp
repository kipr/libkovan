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

#ifndef _DRAW_P_HPP_
#define _DRAW_P_HPP_

#include "build_options.h"

#if BUILD_WITH_QT

#include <QGraphicsView>

class QGraphicsScene;

namespace Private
{
	class Draw : public QGraphicsView
	{
	Q_OBJECT
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
		
		static bool isInited();
		static void init();
	private:
		static bool s_inited;
		QGraphicsScene *m_scene;
	};
}

#else

namespace Private
{
	class Draw
	{
	public:
		Draw() {}
		~Draw() {}
		
		bool open() { return false; }
		void point(const int& x, const int& y) {}
		void line(const int& sx, const int& sy, const int& ex, const int& ey) {}
		void clear() {}
		void setSize(const unsigned int& width, const unsigned int& height) {}
		void raise() {}
		void close() {}
		
		static bool isInited() { return false; }
		static void init() {}
	};
}

#endif

#endif
