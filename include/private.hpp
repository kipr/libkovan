#ifndef _PRIVATE_HPP_
#define _PRIVATE_HPP_

#include <pthread.h>

#define SHARED_MEMORY_KEY 8374

namespace Private
{
	struct SharedMemory
	{
		pthread_mutex_t mutex;
		
		bool motorDirty : 1;
		bool servoDirty : 1;
		
		
		unsigned short rawBatteryVoltage;
	};
}

#endif
