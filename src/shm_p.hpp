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
