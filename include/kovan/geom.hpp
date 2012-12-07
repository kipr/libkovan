#ifndef _GEOM_HPP_
#define _GEOM_HPP_

template<typename T>
class Point2
{
public:
	Point2(const T& x, const T &y)
		: m_x(x),
		m_y(y)
	{
	}
	
	const T &x() const
	{
		return m_x;
	}
	
	const T &y() const
	{
		return m_y;
	}
	
	void setX(const T &x)
	{
		m_x = x;
	}
	
	void setY(const T &y)
	{
		m_y = y;
	}
	
private:
	T m_x;
	T m_y;
};

template<typename T>
class Rectangle
{
public:
	Rectangle(const T &x, const T &y, const T &width, const T &height)
		: m_x(x),
		m_y(y),
		m_width(width),
		m_height(height)
	{
	}
	
	const T &x() const
	{
		return m_x;
	}
	
	const T &y() const
	{
		return m_y;
	}
	
	const T &width() const
	{
		return m_width;
	}
	
	const T &height() const
	{
		return m_height;
	}
	
	void setX(const T &x)
	{
		m_x = x;
	}
	
	void setY(const T &y)
	{
		m_y = y;
	}
	
	void setWidth(const T &width)
	{
		m_width = width;
	}
	
	void setHeight(const T &height)
	{
		m_x = height;
	}
	
private:
	T m_x;
	T m_y;
	T m_width;
	T m_height;
};

#endif
