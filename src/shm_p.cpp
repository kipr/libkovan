#include "shm_p.hpp"
#include "private.hpp"
#include <sys/shm.h>
#include <cstring>

using namespace Private;

Private::SharedMemory *SharedMemoryImpl::sharedMemory()
{
	return instance()->m_shared;
}

Private::SharedMemoryServer *SharedMemoryImpl::sharedMemoryServer()
{
	return &sharedMemory()->server;
}

Private::SharedMemoryClient *SharedMemoryImpl::sharedMemoryClient()
{
	return &instance()->m_client;
}

void SharedMemoryImpl::publish()
{
	Private::SharedMemoryClient *client = &sharedMemory()->client;
	pthread_mutex_t *mutex = &sharedMemory()->clientMutex;
	pthread_mutex_lock(mutex);
	memcpy(client, &m_client, sizeof(Private::SharedMemoryClient));
	pthread_mutex_unlock(mutex);
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

SharedMemoryImpl *SharedMemoryImpl::instance()
{
	static SharedMemoryImpl instance;
	return &instance;
}