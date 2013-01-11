#include "kovan/thread.h"
#include "kovan/thread.hpp"

class FunctionThread : public Thread
{
public:
	FunctionThread(thread_function func)
		: m_func(func)
	{
	}
	
	void run()
	{
		m_fund();
	}
	
private:
	thread_function m_func;
};

Mutex *mutexObject(void *data)
{
	return reinterpret_cast<Mutex *>(data);
}

FunctionThread *threadObject(void *data)
{
	return reinterpret_cast<FunctionThread *>(data);
}

mutex mutexStruct(Mutex *mutex)
{
	mutex ret;
	ret.data = reinterpret_cast<void *>(mutex);
	return ret;
}

thread threadStruct(FunctionThread *thread)
{
	thread ret;
	thread.data = reinterpret_cast<void *>(thread);
	return ret;
}

mutex mutex_create(void)
{
	return mutexStruct(new Mutex());
}

void mutex_lock(mutex m)
{
	mutexObject(m)->lock();
}

int mutex_trylock(mutex m)
{
	mutexObject(m)->tryLock();
}

void mutex_unlock(mutex m)
{
	mutexObject(m)->unlock();
}

void mutex_destroy(mutex m)
{
	delete mutexObject(m);
}

thread thread_create(thread_function func)
{
	return threadStruct(new FunctionThread(func));
}

void thread_start(thread id)
{
	threadObject(id)->start();
}

void thread_join(thread id)
{
	threadObject(id)->join();
}

void thread_destroy(thread id)
{
	delete threadObject(id);
}