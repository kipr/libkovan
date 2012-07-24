#include "draw.h"
#include "draw.hpp"

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