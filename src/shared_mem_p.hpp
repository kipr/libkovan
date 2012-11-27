#ifndef _SHARED_MEM_P_HPP_
#define _SHARED_MEM_P_HPP_

#include <pthread.h>

#define MAX_BUTTON_TEXT_SIZE 16

namespace Private
{
	struct SharedButton
	{
		bool textDirty : 1;
		char text[MAX_BUTTON_TEXT_SIZE];
		bool pressed : 1;
	};
	
	struct SharedData
	{
		SharedButton a;
		SharedButton b;
		SharedButton c;
		SharedButton x;
		SharedButton y;
		SharedButton z;
		
		bool extrasShownDirty : 1;
		bool extrasShown : 1;
	};
	
	struct Shared
	{
		pthread_mutex_t mutex;
		SharedData data;
	};
	
	class SharedMemory
	{
	public:
		SharedMemory();
		~SharedMemory();
		
		bool isConnected() const;
		
		void lock();
		void unlock();
		SharedData *data() const;
		
		static SharedMemory *instance();
		
	private:
		int createSegment(bool &first);
		bool attachSegment(int shmid);
		bool detachSegment();
		bool initializeMutex(pthread_mutex_t *mutex);
		Shared *m_shared;
	};
}

#endif
