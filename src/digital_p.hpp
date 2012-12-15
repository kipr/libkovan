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

#ifndef _DIGITAL_P_HPP_
#define _DIGITAL_P_HPP_

namespace Private
{
	class PublishListener;
	
	class Digital
	{
	public:
		enum Direction {
			In,
			Out,
			Unknown
		};
		
		~Digital();
		
		bool value(const unsigned char& port) const;
		bool setValue(const unsigned char& port, const bool& value);
		
		const Digital::Direction direction(const unsigned char& port) const;
		bool setDirection(const unsigned char& port, const Digital::Direction& direction);
		
		bool pullup(const unsigned char& port) const;
		bool setPullup(const unsigned char& port, const bool& pullup);
		
		static Digital *instance();
	private:
		void wiggle() const;
		
		Digital();
		Digital(const Digital& rhs);
		Digital& operator=(const Digital& rhs);
	};
}

#endif
