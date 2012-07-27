#ifndef _PRIVATE_HPP_
#define _PRIVATE_HPP_

#include <pthread.h>
#include <sys/time.h>

#define SHARED_MEMORY_KEY 8374

#define NUM_DIGITALS 8
#define NUM_ANALOGS 8
#define NUM_MOTORS 4
#define NUM_SERVOS 4
#define MAX_BUTTON_TEXT_SIZE 16

namespace Private
{
	// Server writes this data. Clients read.
	struct SharedMemoryServer
	{
		timeval timestamp;
		unsigned long updates;
		unsigned short servoPositions[NUM_SERVOS];
		
		bool digitals[NUM_DIGITALS];
		unsigned short analogs[NUM_ANALOGS];
		
		unsigned short backEMFs[NUM_MOTORS * 2];

		unsigned short rawBatteryVoltage;
	};
	
	struct PID {
		short p;
		short i;
		short d;
		short pd;
		short id;
		short dd;
	};
	
	enum MotorControlMode {
		PID = 0,
		PWM
	};
	
	// Clients write this data. Server reads.
	struct SharedMemoryClient
	{
		bool motorDirty : 1;
		
		bool motorControlModeDirty : 1;
		MotorControlMode motorControlMode;
		
		unsigned char pwmsDirty : NUM_MOTORS; // Lower 4 bits used
		unsigned char pwms[NUM_MOTORS];
		
		unsigned char pidsDirty : NUM_MOTORS; // Lower 4 bits used
		PID pids[NUM_MOTORS];
		
		unsigned char servoDirty : NUM_SERVOS;
		unsigned short servoPositions[NUM_SERVOS];
		
		bool pullupDirty[NUM_ANALOGS];
		bool pullup[NUM_ANALOGS];
	};
	
	struct SharedButton
	{
		bool textDirty : 1;
		char text[MAX_BUTTON_TEXT_SIZE];
		bool pressed : 1;
	};
	
	// Clients read and write this data;
	struct SharedMemoryInterClient
	{
		SharedButton a;
		SharedButton b;
		SharedButton c;
		
		bool isShowExtraButtonsDirty : 1;
		bool showExtraButtons : 1;
		
		SharedButton x;
		SharedButton y;
		SharedButton z;
	};
	
	struct SharedMemory
	{
		pthread_mutex_t serverMutex;
		SharedMemoryServer server;
		
		pthread_mutex_t clientMutex;
		SharedMemoryClient client;
		
		pthread_mutex_t interClientMutex;
		SharedMemoryInterClient interClient;
	};
	
	bool initSharedMemory(Private::SharedMemory *shm);
}

#endif
