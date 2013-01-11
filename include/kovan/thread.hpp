#ifndef _THREAD_HPP_
#define _THREAD_HPP_

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();
	
	void lock();
	bool tryLock();
	
	void unlock();
	
private:
	Mutex(const Mutex &rhs);

#ifdef WIN32
	HANDLE m_handle;
#else
	pthread_mutex_t m_handle;
#endif
};

class Thread
{
public:
	Thread();
	virtual ~Thread();
	
	void start();
	void join();
	
	virtual void run() = 0;
	
private:
#ifndef WIN32
	pthread_t m_thread;
#else
	DWORD m_thread;
#endif
};

#endif
