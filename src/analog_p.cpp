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

#include "analog_p.hpp"
#include "shm_p.hpp"

using namespace Private;

namespace Private
{
	class AnalogPublishListener : public PublishListener
	{
	public:
		virtual void published(Private::SharedMemoryClient *client)
		{
			client->analogPullupsDirty = 0;
		}
	};
}


Analog::~Analog()
{
	delete m_listener;
}

void Analog::setPullup(const unsigned char& port, const bool& pullup)
{
	if(port < 8 || port > 15) return;
	
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm) return;
	
	shm->analogPullupsDirty &= 1 << (NUM_ANALOGS - 1 - port);
	shm->analogPullups &= pullup << (NUM_ANALOGS - 1 - port);
	
	SharedMemoryImpl::instance()->doAutoPublish();
}

bool Analog::pullup(const unsigned char& port) const
{
	if(port >= NUM_ANALOGS) return false;
	
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	return shm->analogPullups & (NUM_ANALOGS - 1 - port);
}

unsigned short Analog::value(const unsigned char& port) const
{
	if(port >= NUM_ANALOGS) return 0xFFFF;
	
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	return shm->analogs[port];
}

Analog *Analog::instance()
{
	static Analog s_analog;
	return &s_analog;
}

Analog::Analog() : m_listener(new AnalogPublishListener())
{
	SharedMemoryImpl::instance()->addPublishListener(m_listener);
}

Analog::Analog(const Analog& ) {}
Analog& Analog::operator=(const Analog&) { return *this; }