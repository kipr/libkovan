#include "thread.hpp"

Mutex::Mutex()
{
#ifdef WIN32
	m_handle = CreateMutex(NULL, FALSE, NULL);
#else
	pthread_mutex_create(&m_handle, NULL);
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
	
}

bool Mutex::tryLock()
{
	
}

void Mutex::unlock()
{
#ifdef WIN32
	
#else
	pthread_mutex_unlock();
#endif
}

Mutex::Mutex(const Mutex &)
{
}

extern "C"
{
	static void __runThread(void *data)
	{
		Thread *t = reinterpret_cast<Thread *>(data);
		t->run();
	}
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
	pthread_destroy(&m_thread);
#endif
}

void Thread::start()
{
#ifdef WIN32

#else
	pthread_create(&m_thread, NULL, __runThread,
		reinterpret_cast<void *>(this));
#endif
}

void Thread::join()
{
#ifdef WIN32
	
#else
	pthread_join(m_thread);
#endif
}