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

#ifndef _BUTTON_P_HPP_
#define _BUTTON_P_HPP_

#include "kovan/button_ids.hpp"

#define MAX_BUTTON_TEXT_SIZE 16

namespace Private
{
	struct SharedButton;
	
	class Button
	{
	public:
		void setText(const ::Button::Type::Id &id, const char *text);
		bool isTextDirty(const ::Button::Type::Id &id) const;
		const char *text(const ::Button::Type::Id &id) const;
		void setPressed(const ::Button::Type::Id &id, bool pressed);
		bool isPressed(const ::Button::Type::Id &id) const;
		void resetButtons();
		
		void setExtraShown(const bool& shown);
		bool isExtraShown() const;
		
		static Button *instance();
	private:
		unsigned char buttonOffset(const ::Button::Type::Id &id) const;
		bool buttonRegs(unsigned short &start, unsigned short &end, ::Button::Type::Id id) const;
		
		Button();
		Button(const Button& rhs);
		
		mutable char m_text[6][MAX_BUTTON_TEXT_SIZE];
	};
}

#endif
