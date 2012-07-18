#include "shm_p.hpp"
#include "private.hpp"
#include <sys/shm.h>

using namespace Private;

Private::SharedMemory *SharedMemoryImpl::sharedMemory()
{
	static SharedMemoryImpl impl;
	return impl.m_shared;
}
	

SharedMemoryImpl::SharedMemoryImpl()
	: m_shared(0)
{
	static size_t size = sizeof(SharedMemory);
	int shmid = shmget(SHARED_MEMORY_KEY, size, IPC_CREAT | 0x1b6);
	if(shmid < 0) return;
	
	// Attach to current shared memory segment.
	void *shm = shmat(shmid, 0, 0);
	if((void *) -1 == shm) return;
	
	m_shared = reinterpret_cast<Private::SharedMemory *>(shm);
}

SharedMemoryImpl::~SharedMemoryImpl()
{
	if(m_shared) shmdt(m_shared);
}
