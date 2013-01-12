#ifndef _THREAD_H_
#define _THREAD_H_

typedef struct
{
	void *data;
} mutex;

typedef struct
{
	void *data;
} thread;

typedef void (*thread_function)();

mutex mutex_create(void);
void mutex_lock(mutex m);
int mutex_trylock(mutex m);
void mutex_unlock(mutex m);
void mutex_destroy(mutex m);

thread thread_create(thread_function func);
void thread_start(thread id);
void thread_wait(thread id);
void thread_destroy(thread id);

#endif
