#include "common_p.hpp"
#include <cstring>

using namespace Private;

Dev::Dev(const char *dev)
	: m_dev(0), m_handle(0)
{
	const size_t len = strlen(dev);
	m_dev = new char[len + 1];
	memcpy(m_dev, dev, len);
	m_dev[len] = 0;
}

Dev::~Dev()
{
	close();
	delete[] m_dev;
}

const char *Dev::dev() const
{
	return m_dev;
}

bool Dev::open()
{
	if(m_handle) return true;
	m_handle = fopen(m_dev, "r");
	return m_handle;
}

bool Dev::close()
{
	if(!m_handle) return false;
	fclose(m_handle);
	m_handle = 0;
	return true;
}

size_t Dev::read(unsigned char *ptr, const size_t& size, const size_t& count)
{
	if(!m_handle) return 0;
	return ::fread(ptr, size, count, m_handle);
}