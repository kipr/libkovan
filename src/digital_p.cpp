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

#include "digital_p.hpp"
#include "shm_p.hpp"

using namespace Private;

namespace Private
{
	class DigitalPublishListener : public PublishListener
	{
	public:
		virtual void published(Private::SharedMemoryClient *client)
		{
			client->digitalPullupsDirty = 0;
			client->digitalDirectionsDirty = 0;
			client->digitalsDirty = 0;
		}
	};
}

Digital::~Digital()
{
	delete m_listener;
}

bool Digital::value(const unsigned char& port) const
{
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm || port >= NUM_DIGITALS) return false;
	return shm->digitals & (1 << (NUM_DIGITALS - 1 - port));
}

bool Digital::setValue(const unsigned char& port, const bool& value)
{
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port >= NUM_DIGITALS) return false;
	
	shm->digitalsDirty |= 1 << (NUM_DIGITALS - 1 - port);
	if(value) shm->digitals |= 1 << (NUM_DIGITALS - 1 - port);
	else shm->digitals &= ~(1 << (NUM_DIGITALS - 1 - port));
	SharedMemoryImpl::instance()->doAutoPublish();
	return true;
}

const Digital::Direction Digital::direction(const unsigned char& port) const
{
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm || port >= NUM_DIGITALS) return Unknown;
	return shm->digitalDirections & (1 << (NUM_DIGITALS - 1 - port)) ? Out : In;
}

bool Digital::setDirection(const unsigned char& port, const Digital::Direction& direction)
{
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port >= NUM_DIGITALS) return false;
	
	shm->digitalDirectionsDirty |= 1 << (NUM_DIGITALS - 1 - port);
	shm->digitalDirections |= (direction == In ? 0 : 1) << (NUM_DIGITALS - 1 - port);
	SharedMemoryImpl::instance()->doAutoPublish();
	
	return true;
}

bool Digital::pullup(const unsigned char& port) const
{
	SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm || port > NUM_DIGITALS - 1) return false;
	return shm->digitalPullups & (1 << (NUM_DIGITALS - 1 - port));
}

bool Digital::setPullup(const unsigned char& port, const bool& pullup)
{
	SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm || port >= NUM_DIGITALS) return false;
	
	shm->digitalPullupsDirty |= 1 << (NUM_DIGITALS - 1 - port);
	shm->digitalPullups |= pullup << (NUM_DIGITALS - 1 - port);
	SharedMemoryImpl::instance()->doAutoPublish();
	return true;
}

Digital *Digital::instance()
{
	static Digital s_digital;
	return &s_digital;
}

Digital::Digital()
	: m_listener(new DigitalPublishListener())
{
	SharedMemoryImpl::instance()->addPublishListener(m_listener);
}

Digital::Digital(const Digital&) {}
Digital& Digital::operator=(const Digital&) { return *this; }