#include "kovan/thread.hpp"

#include <pthread.h>

Mutex::Mutex()
{
#ifdef WIN32
	m_handle = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_init(&m_handle, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef WIN32
	CloseHandle(m_handle);
#else
	pthread_mutex_destroy(&m_handle);
#endif
}

void Mutex::lock()
{
#ifdef WIN32
	
#else
	pthread_mutex_lock(&m_handle);
#endif
}

bool Mutex::tryLock()
{
	
}

void Mutex::unlock()
{
#ifdef WIN32
	
#else
	pthread_mutex_unlock(&m_handle);
#endif
}

Mutex::Mutex(const Mutex &)
{
}

void *__runThread(void *data)
{
	Thread *t = reinterpret_cast<Thread *>(data);
	t->run();
	return NULL;
}


Thread::Thread()
#ifdef WIN32
	: m_thread(INVALID_HANDLE)
#endif
{
	
}

Thread::~Thread()
{
#ifdef WIN32
	
#else
	pthread_cancel(m_thread);
#endif
}

void Thread::start()
{
#ifdef WIN32

#else
	pthread_create(&m_thread, NULL, &__runThread,
		reinterpret_cast<void *>(this));
#endif
}

void Thread::join()
{
#ifdef WIN32
	
#else
	pthread_join(m_thread, NULL);
#endif
}