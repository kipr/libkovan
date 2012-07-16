#ifndef _COMMON_P_HPP_
#define _COMMON_P_HPP_

#include <cstdio>

#define DEV(x) ("/dev/kovan/" #x)

namespace Private
{
	class Dev
	{
	public:
		Dev(const char *dev);
		~Dev();
	
		const char *dev() const;
		
		bool open();
		
		inline bool isOpen() const
		{
			return m_handle;
		}
		
		bool close();
	
		size_t read(unsigned char *ptr, const size_t& size, const size_t& count);
	
		template<typename T>
		inline size_t read(T* ptr, const size_t& count = 1) {
			return read((unsigned char *)ptr, sizeof(T), count);
		}
	private:
		char *m_dev;
		FILE *m_handle;
	};
}

#endif
