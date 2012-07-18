#ifndef _SHM_P_HPP_
#define _SHM_P_HPP_

namespace Private
{
	struct SharedMemory;
	
	class SharedMemoryImpl
	{
	public:
		static Private::SharedMemory *sharedMemory();
		
	private:
		SharedMemoryImpl();
		~SharedMemoryImpl();
		
		Private::SharedMemory *m_shared;
	};
}

#endif
