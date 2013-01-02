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
#include "shared_mem_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"

#include <cstring>
#include <pthread.h>
#include <cstdio>

using namespace Private;

void Private::Button::setText(const ::Button::Type::Id &id, const char *text)
{
	if(!Private::SharedMemory::instance()->isConnected()) return;
	
	SharedButton *button = selectButton(id);
	if(!button) return;
	
	Private::SharedMemory::instance()->lock();
	memcpy(button->text, text, MAX_BUTTON_TEXT_SIZE);
	button->text[MAX_BUTTON_TEXT_SIZE - 1] = 0;
	button->textDirty = 1;
	Private::SharedMemory::instance()->unlock();
}

bool Private::Button::isTextDirty(const ::Button::Type::Id &id) const
{
	if(!Private::SharedMemory::instance()->isConnected()) return false;
	SharedButton *button = selectButton(id);
	if(!button) return false;
	const bool value = button->textDirty;
	if(value) {
		Private::SharedMemory::instance()->lock();
		button->textDirty = 0;
		Private::SharedMemory::instance()->unlock();
	}
	return value;
}

const char *Private::Button::text(const ::Button::Type::Id &id) const
{
	if(!Private::SharedMemory::instance()->isConnected()) return 0;
	
	SharedButton *button = selectButton(id);
	if(!button) return 0;
	
	return button->text;
}

void Private::Button::setPressed(const ::Button::Type::Id &id, bool pressed)
{
	if(!Private::SharedMemory::instance()->isConnected()) return;
	
	SharedButton *button = selectButton(id);
	if(!button) return;
	
	Private::SharedMemory::instance()->lock();
	button->pressed = pressed ? 1 : 0;
	Private::SharedMemory::instance()->unlock();
}

bool Private::Button::isPressed(const ::Button::Type::Id &id) const
{
	if(id == ::Button::Type::Side) {
		return Private::Kovan::instance()->currentState().t[SIDE_BUTTON];
	}
	
	if(!Private::SharedMemory::instance()->isConnected()) return false;
	SharedButton *button = selectButton(id);
	if(!button) return false;
	return button->pressed;
}

void Private::Button::setExtraShown(const bool& shown)
{
	if(!Private::SharedMemory::instance()->isConnected()) return;
	
	Private::SharedMemory::instance()->lock();
	Private::SharedMemory::instance()->data()->extrasShown = shown ? 1 : 0;
	Private::SharedMemory::instance()->data()->extrasShownDirty = 1;
	Private::SharedMemory::instance()->unlock();
}

bool Private::Button::isExtraShown() const
{
	return Private::SharedMemory::instance()->data()->extrasShown;
}

bool Private::Button::isExtraShownDirty() const
{
	if(!Private::SharedMemory::instance()->isConnected()) return false;
	const bool value = Private::SharedMemory::instance()->data()->extrasShownDirty;
	if(value) {
		Private::SharedMemory::instance()->lock();
		Private::SharedMemory::instance()->data()->extrasShownDirty = 0;
		Private::SharedMemory::instance()->unlock();
	}
	return value;
}

Private::Button *Private::Button::instance()
{
	static Button s_button;
	return &s_button;
}

Private::SharedButton *Private::Button::selectButton(const ::Button::Type::Id &id) const
{
	switch(id) {
	case ::Button::Type::A:
		return &Private::SharedMemory::instance()->data()->a;
	case ::Button::Type::B:
		return &Private::SharedMemory::instance()->data()->b;
	case ::Button::Type::C:
		return &Private::SharedMemory::instance()->data()->c;
	case ::Button::Type::X:
		return &Private::SharedMemory::instance()->data()->x;
	case ::Button::Type::Y:
		return &Private::SharedMemory::instance()->data()->y;
	case ::Button::Type::Z:
		return &Private::SharedMemory::instance()->data()->z;
	}
	return 0;
}

Private::Button::Button()
{
}

Private::Button::Button(const Button& rhs)
{
}