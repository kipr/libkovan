#ifndef _DRAW_P_HPP_
#define _DRAW_P_HPP_

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
		
	private:
		QGraphicsScene *m_scene;
	};
}

#endif
