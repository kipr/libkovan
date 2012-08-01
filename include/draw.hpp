/*!
 * \file draw.hpp
 * \brief Classes for drawing to the screen
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup draw Drawing
 */

#ifndef _DRAW_HPP_
#define _DRAW_HPP_

namespace Private
{
	class Draw;
}

class Draw
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
