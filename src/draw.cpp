#include "kovan/draw.hpp"
#include "draw_p.hpp"

Draw::Draw()
	: p_draw(
#if BUILD_WITH_DRAW_P
	new Private::Draw()
#else
	0
#endif
	)
{
}

Draw::~Draw()
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	delete p_draw;
#endif
}

bool Draw::open()
{
	if(!p_draw) return false;
#if BUILD_WITH_DRAW_P
	return p_draw->open();
#endif
}

void Draw::point(int x, int y)
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	p_draw->point(x, y);
#endif
}

void Draw::line(int sx, int sy, int ex, int ey)
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	p_draw->line(sx, sy, ex, ey);
#endif
}

void Draw::clear()
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	p_draw->clear();
#endif
}

void Draw::setSize(const unsigned int& width, const unsigned int& height)
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	p_draw->setSize(width, height);
#endif
}

void Draw::raise()
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	p_draw->raise();
#endif
}

void Draw::close()
{
	if(!p_draw) return;
#if BUILD_WITH_DRAW_P
	p_draw->close();
#endif
}