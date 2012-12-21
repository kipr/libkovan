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

#ifndef _ANALOG_P_HPP_
#define _ANALOG_P_HPP_

namespace Private
{
	class PublishListener;
	
	class Analog
	{
	public:
		~Analog();
		
		void setPullup(const unsigned char& port, const bool& pullup);
		bool pullup(const unsigned char& port) const;
		unsigned short value(const unsigned char& port) const;
		// TODO: Move?
		bool isCharging() const;
		static Analog *instance();
		
	private:
		Analog();
		Analog(const Analog& rhs);
		Analog& operator=(const Analog& rhs);
	};
}

#endif
