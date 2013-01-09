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

#ifndef _KOVAN_MODULE_P_HPP_
#define _KOVAN_MODULE_P_HPP_

#include "kovan_command_p.hpp"

#include <vector>

#ifndef _WIN32
#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#else
#define _WIN32_WINNT 0x0501
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif
#include <stdint.h>

namespace Private
{
	typedef std::vector<Command> CommandVector;
	
	class KovanModule
	{
	public:
		KovanModule(const uint64_t& moduleAddress, const uint16_t& modulePort);
		~KovanModule();

		bool init();
		bool bind(const uint64_t& address, const uint16_t& port);
		void close();

		uint64_t moduleAddress() const;
		uint16_t modulePort() const;

		bool send(const Command& command);
		bool send(const CommandVector& commands);

		bool recv(State& state);

		int getState(State &state);
		void displayState(const State &state);

	private:
		int m_sock;
		sockaddr_in m_out;

		static Packet *createPacket(const uint16_t& num, uint32_t& packet_size);
	};
}

#endif
