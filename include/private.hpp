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
		timeval timestamp;
		unsigned long updates;
		unsigned short servoPositions[4];
		unsigned short rawBatteryVoltage;
	};
	
	// Clients write this data. Server reads.
	struct SharedMemoryClient
	{
		bool motorDirty : 1;
		unsigned char servoDirty : 4;
		unsigned short servoPositions[4];
	};
	
	struct SharedMemory
	{
		pthread_mutex_t serverMutex;
		SharedMemoryServer server;
		
		pthread_mutex_t clientMutex;
		SharedMemoryClient client;
	};
}

#endif
