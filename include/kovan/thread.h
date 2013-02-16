#ifndef _THREAD_H_
#define _THREAD_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
	void *data;
} mutex;

typedef struct
{
	void *data;
} thread;

typedef void (*thread_function)();

EXPORT_SYM mutex mutex_create(void);
EXPORT_SYM void mutex_lock(mutex m);
EXPORT_SYM int mutex_trylock(mutex m);
EXPORT_SYM void mutex_unlock(mutex m);
EXPORT_SYM void mutex_destroy(mutex m);

EXPORT_SYM thread thread_create(thread_function func);
EXPORT_SYM void thread_start(thread id);
EXPORT_SYM void thread_wait(thread id);
EXPORT_SYM void thread_destroy(thread id);

#ifdef __cplusplus
}
#endif

#endif
