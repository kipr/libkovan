#ifndef _PRIVATE_HPP_
#define _PRIVATE_HPP_

#include <pthread.h>
#include <sys/time.h>

#define SHARED_MEMORY_KEY 8374

namespace Private
{
	// Server writes this data. Clients read.
	struct SharedMemoryServer
	{
		pthread_mutex_t mutex;
		timeval timestamp;
		unsigned long updates;
		unsigned short servoPositions[4];
		unsigned short rawBatteryVoltage;
	};
	
	// Clients write this data. Server reads.
	struct SharedMemoryClient
	{
		pthread_mutex_t mutex;
		bool motorDirty : 1;
		unsigned char servoDirty : 4;
		unsigned short servoPositions[4];
	};
	
	struct SharedMemory
	{
		SharedMemoryServer server;
		SharedMemoryClient client;
	};
}

#endif
