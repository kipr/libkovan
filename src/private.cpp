/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

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