#include "kovan/geom.h"

point2 create_point2(int x, int y)
{
	point2 ret;
	ret.x = x;
	ret.y = y;
	return ret;
}

rectangle create_rectangle(int x, int y, int width, int height)
{
	rectangle ret;
	ret.x = x;
	ret.y = y;
	ret.width = width;
	ret.height = height;
	return ret;
}