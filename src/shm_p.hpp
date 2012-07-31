#ifndef _SHM_P_HPP_
#define _SHM_P_HPP_

#include <pthread.h>
#include <vector>
#include "private.hpp"

namespace Private
{
	class PublishListener
	{
	public:
		virtual ~PublishListener();
		virtual void published(Private::SharedMemoryClient *client) = 0;
	};
	
	class SharedMemoryImpl
	{
	public:
		Private::SharedMemory *sharedMemory();
		Private::SharedMemoryServer *sharedMemoryServer();
		Private::SharedMemoryClient *sharedMemoryClient();
		Private::SharedMemoryInterClient *sharedMemoryInterClient();
		
		void addPublishListener(PublishListener *listener);
		void removePublishListener(PublishListener *listener);
		
		void setAutoPublish(const bool& on);
		bool doesAutoPublish() const;
		
		void doAutoPublish();
		
		void publish();
		bool isErrorState() const;
		
		static SharedMemoryImpl *instance();
	private:
		
		SharedMemoryImpl();
		~SharedMemoryImpl();
		
		Private::SharedMemory *m_shared;
		Private::SharedMemoryClient m_client;
		
		bool m_autoPublish;
		
		std::vector<PublishListener *> m_listeners;
	};
}

#endif
