#ifndef _SHM_P_HPP_
#define _SHM_P_HPP_

#include <pthread.h>
#include "private.hpp"

namespace Private
{
	class SharedMemoryImpl
	{
	public:
		Private::SharedMemory *sharedMemory();
		Private::SharedMemoryServer *sharedMemoryServer();
		Private::SharedMemoryClient *sharedMemoryClient();
		void publish();
		
		bool isErrorState() const;
		
		static SharedMemoryImpl *instance();
	private:
		
		SharedMemoryImpl();
		~SharedMemoryImpl();
		
		Private::SharedMemory *m_shared;
		Private::SharedMemoryClient m_client;
	};
}

#endif
