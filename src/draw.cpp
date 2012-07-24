#include "draw.hpp"
#include "draw_p.hpp"

Draw::Draw()
	: p_draw(0)
{
	if(!Private::Draw::isInited()) Private::Draw::init();
	p_draw = new Private::Draw();
}

Draw::~Draw()
{
	delete p_draw;
}

bool Draw::open()
{
	return p_draw->open();
}

void Draw::point(int x, int y)
{
	p_draw->point(x, y);
}

void Draw::line(int sx, int sy, int ex, int ey)
{
	p_draw->line(sx, sy, ex, ey);
}

void Draw::clear()
{
	p_draw->clear();
}

void Draw::setSize(const unsigned int& width, const unsigned int& height)
{
	p_draw->setSize(width, height);
}

void Draw::raise()
{
	p_draw->raise();
}

void Draw::close()
{
	p_draw->close();
}