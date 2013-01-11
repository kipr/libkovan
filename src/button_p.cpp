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

#include "button_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"

#include <cstring>
#include <cstdio>
#include <iostream>

using namespace Private;

void Private::Button::setText(const ::Button::Type::Id &id, const char *text)
{
	const unsigned char offset = buttonOffset(id);
	if(offset > 5) return;
	
	strncpy(m_text[offset], text, MAX_BUTTON_TEXT_SIZE);
	m_text[offset][MAX_BUTTON_TEXT_SIZE - 1] = 0;
	
	unsigned short start = 0;
	unsigned short end = 0;
	if(!buttonRegs(start, end, id)) return;
	
	for(int j = 0; j < MAX_BUTTON_TEXT_SIZE; j += 2) {
		Private::Kovan::instance()->enqueueCommand(createWriteCommand(start + j / 2,
			m_text[offset][j] << 8 | m_text[offset][j + 1]), false);
	}
	
	unsigned short &dirty = Private::Kovan::instance()->currentState().t[BUTTON_TEXT_DIRTY];
	dirty |= 1 << offset;
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(BUTTON_TEXT_DIRTY, dirty));
}

bool Private::Button::isTextDirty(const ::Button::Type::Id &id) const
{
	Private::Kovan::instance()->autoUpdate();
	const unsigned char offset = buttonOffset(id);
	if(offset > 5) return false;
	unsigned short &dirty = Private::Kovan::instance()->currentState().t[BUTTON_TEXT_DIRTY];
	if(!((dirty >> offset) & 1)) return false;
	dirty &= ~(1 << offset);
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(BUTTON_TEXT_DIRTY, dirty));
	return true;
}

const char *Private::Button::text(const ::Button::Type::Id &id) const
{
	const unsigned char offset = buttonOffset(id);
	if(offset >= 6) return 0;
	
	unsigned short start = 0;
	unsigned short end = 0;
	if(!buttonRegs(start, end, id)) return 0;
	
	unsigned short *registers = Private::Kovan::instance()->currentState().t;
	unsigned char j = 0;
	for(unsigned short i = start; j < MAX_BUTTON_TEXT_SIZE && i <= end; ++i) {
		m_text[offset][j++] = (registers[i] >> 8) & 0x00FF;
		m_text[offset][j++] = registers[i] & 0x00FF;
	}
	m_text[offset][MAX_BUTTON_TEXT_SIZE - 1] = 0;
	return m_text[offset];
}

void Private::Button::setPressed(const ::Button::Type::Id &id, bool pressed)
{
	const unsigned char offset = buttonOffset(id);
	if(offset > 5) return;
	unsigned short &states = Private::Kovan::instance()->currentState().t[BUTTON_STATES];
	if(pressed) states |= (1 << offset);
	else states &= ~(1 << offset);
	std::cout << "States: " << std::hex << states << std::endl;
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(BUTTON_STATES, states));
}

bool Private::Button::isPressed(const ::Button::Type::Id &id) const
{
	Private::Kovan::instance()->autoUpdate();
	if(id == ::Button::Type::Side) {
		return Private::Kovan::instance()->currentState().t[SIDE_BUTTON];
	}
	
	const unsigned char offset = buttonOffset(id);
	if(offset > 5) return false;
	unsigned short &states = Private::Kovan::instance()->currentState().t[BUTTON_STATES];
	// std::cout << "states: " << std::hex << states << std::endl;
	return (states >> offset) & 1;
}

void Private::Button::setExtraShown(const bool& shown)
{
	unsigned short &states = Private::Kovan::instance()->currentState().t[BUTTON_STATES];
	unsigned short oldStates = states;
	if(shown) states |= 0x8000;
	else states &= 0x7FFF;
	if(oldStates == states) return;
	Private::Kovan::instance()->enqueueCommand(createWriteCommand(BUTTON_STATES, states));
}

bool Private::Button::isExtraShown() const
{
	Private::Kovan::instance()->autoUpdate();
	return Private::Kovan::instance()->currentState().t[BUTTON_STATES] & 0x8000;
}

void Private::Button::resetButtons()
{
	strncpy(m_text[0], "A", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[1], "B", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[2], "C", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[3], "X", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[4], "Y", MAX_BUTTON_TEXT_SIZE);
	strncpy(m_text[5], "Z", MAX_BUTTON_TEXT_SIZE);
}

Private::Button *Private::Button::instance()
{
	static Button s_button;
	return &s_button;
}

unsigned char Private::Button::buttonOffset(const ::Button::Type::Id &id) const
{
	return (unsigned char)id;
}

bool Private::Button::buttonRegs(unsigned short &start, unsigned short &end, ::Button::Type::Id id) const
{
	switch(id) {
	case ::Button::Type::A:
		start = BUTTON_A_TEXT_START;
		end = BUTTON_A_TEXT_END;
		break;
	case ::Button::Type::B:
		start = BUTTON_B_TEXT_START;
		end = BUTTON_B_TEXT_END;
		break;
	case ::Button::Type::C:
		start = BUTTON_C_TEXT_START;
		end = BUTTON_C_TEXT_END;
		break;
	case ::Button::Type::X:
		start = BUTTON_X_TEXT_START;
		end = BUTTON_X_TEXT_END;
		break;
	case ::Button::Type::Y:
		start = BUTTON_Y_TEXT_START;
		end = BUTTON_Y_TEXT_END;
		break;
	case ::Button::Type::Z:
		start = BUTTON_Z_TEXT_START;
		end = BUTTON_Z_TEXT_END;
		break;
	default: return false;
	}
	return true;
}

Private::Button::Button()
{
	resetButtons();
}

Private::Button::Button(const Button& rhs)
{
}
