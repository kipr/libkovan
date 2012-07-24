#include "private.hpp"
#include <sys/shm.h>
#include <pthread.h>

namespace Private
{
	bool initMutex(pthread_mutex_t *mutex)
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
}

bool Private::initSharedMemory(Private::SharedMemory *shm)
{
	bool success = true;
	success &= Private::initMutex(&shm->serverMutex);
	success &= Private::initMutex(&shm->clientMutex);
	success &= Private::initMutex(&shm->interClientMutex);
	return success;
}