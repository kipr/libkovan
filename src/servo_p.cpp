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

#include "servo_p.hpp"
#include "shm_p.hpp"
#include "private.hpp"
#include <cstdio>

namespace Private
{
	class ServoPublishListener : public PublishListener
	{
	public:
		virtual void published(Private::SharedMemoryClient *client)
		{
			static unsigned char i = 0;
			if(i++ == 0) printf("Cleared servo dirty bit");
			client->servoDirty = 0;
		}
	};
}

Private::Servo::Servo()
	: m_listener(new ServoPublishListener())
{
	SharedMemoryImpl::instance()->addPublishListener(m_listener);
}

Private::Servo::~Servo()
{
	delete m_listener;
}

bool Private::Servo::setPosition(const port_t& port, const unsigned short& position)
{
	Private::SharedMemoryClient *shm = SharedMemoryImpl::instance()->sharedMemoryClient();
	if(!shm) return false;
	if(port < 1 || port > 4) return false;
	shm->servoPositions[port - 1] = position & 0x3FF;
	shm->servoDirty |= 1 << (4 - port);
	SharedMemoryImpl::instance()->doAutoPublish();
	return true;
}

unsigned short Private::Servo::position(const port_t& port) const
{
	Private::SharedMemoryServer *shm = SharedMemoryImpl::instance()->sharedMemoryServer();
	if(!shm) return false;
	if(port < 1 || port > 4) return false;
	unsigned short pos = shm->servoPositions[port - 1] & 0x3FF;
	return pos;
}

Private::Servo *Private::Servo::instance()
{
	static Servo instance;
	return &instance;
}