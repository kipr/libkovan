#include "build_options.h"

#if BUILD_WITH_QT

#include "draw_p.hpp"
#include <QGraphicsScene>

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

#endif