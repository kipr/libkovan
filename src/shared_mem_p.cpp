#include "shared_mem_p.hpp"
#include <sys/types.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>

#define SHM_KEY 8374

Private::SharedMemory::SharedMemory()
{
	bool first = false;
	int shmid = createSegment(first);
	if(shmid < 0) {
		perror("shmget");
		return;
	}
	if(!attachSegment(shmid)) {
		perror("shmat");
		return;
	}
	if(first) initializeMutex(&m_shared->mutex);
}

Private::SharedMemory::~SharedMemory()
{
	detachSegment();
}

Private::SharedMemory *Private::SharedMemory::instance()
{
	static Private::SharedMemory s_shm;
	return &s_shm;
}

bool Private::SharedMemory::isConnected() const
{
	return m_shared;
}

void Private::SharedMemory::lock()
{
	pthread_mutex_lock(&m_shared->mutex);
}

void Private::SharedMemory::unlock()
{
	pthread_mutex_unlock(&m_shared->mutex);
}

Private::SharedData *Private::SharedMemory::data() const
{
	return &m_shared->data;
}

int Private::SharedMemory::createSegment(bool &first)
{
	// 1024 is an arbitrarily high value
	int ret = -1;
	first = false;
	if((ret = shmget(SHM_KEY, 1024, 0666)) < 0) {
		if(errno != ENOENT) return ret;
		if((ret = shmget(SHM_KEY, 1024, 0666 | IPC_CREAT)) < 0) return -1;
		first = true;
	}
	return ret;
}

bool Private::SharedMemory::attachSegment(int shmid)
{
	m_shared = reinterpret_cast<Private::Shared *>(shmat(shmid, 0, 0));
	const bool success = m_shared != (void *)-1;
	if(!success) m_shared = 0;
	return success;
}

bool Private::SharedMemory::detachSegment()
{
	if(!m_shared) return true;
	const bool success = !shmdt(m_shared);
	if(success) m_shared = 0;
	return success;
}

bool Private::SharedMemory::initializeMutex(pthread_mutex_t *mutex)
{
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
#ifdef linux
	if(!pthread_mutexattr_setrobust_np(&attr, PTHREAD_MUTEX_ROBUST_NP)) return false;
#endif
	if(!pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED)) return false;
	if(!pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK)) return false;
	if(!pthread_mutex_init(mutex, &attr)) return false;
	pthread_mutexattr_destroy(&attr);
	return true;
}