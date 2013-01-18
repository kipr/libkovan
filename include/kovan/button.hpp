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

/*!
 * \file button.hpp
 * \brief Classes for manipulating and using buttons
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup button Buttons
 */

#ifndef _BUTTON_HPP_
#define _BUTTON_HPP_

#include "sensor.hpp"
#include "button_ids.hpp"
 #include "export.h"
 

/*!
 * \class AbstractButton
 * \brief The base class for all buttons
 * \details Provides several convenience methods for working with buttons
 * \ingroup button
 */
class EXPORT_SYM AbstractButton : public Sensor<bool>
{
public:
	virtual ~AbstractButton();
	virtual void setPressed(bool pressed) = 0;
	
	/*!
	 * \return true if the button is pressed, false if it is not pressed.
	 * \see isNotPressed
	 * \see isClicked
	 */
	inline bool isPressed() const { return value(); };
	
	/*!
	 * \return true if the button is not pressed, false if it is pressed.
	 * \see isPressed
	 */
	inline bool isNotPressed() const { return !isPressed(); }
	
	/*!
	 * Gets the current state of the butten (pressed or not pressed.)
	 * \return true if the button is pressed, false if it is not
	 * \note This function blocks until the button is no longer pressed.
	 * \see isPressed
	 * \see waitUntilReleased
	 * \blocks
	 */
	inline bool isClicked() const
	{
		const bool ret = isPressed();
		waitUntilReleased();
		return ret;
	}
	
	
	
	/*!
	 * Blocks until the button is released. Returns immeadiately if the button is not pressed.
	 * \blocks
	 */
	virtual void waitUntilReleased() const;
	
	/*!
	 * Blocks until the button is pressed. Returns immeadiately if the button is pressed.
	 * \blocks
	 */
	virtual void waitUntilPressed() const;
	
	/*!
	 * Blocks until the button is clicked.
	 * \blocks
	 */
	virtual void waitUntilClicked() const;
};

/*!
 * \class AbstractTextButton
 * \brief The base class for all buttons that have text
 * \details Provides methods to access and manipulate the button's text
 * \ingroup button
 */
class EXPORT_SYM AbstractTextButton : public AbstractButton
{
public:
	virtual ~AbstractTextButton();
	
	/*!
	 * Sets the button's current text
	 * Strings over 16 bytes will be trancated.
	 * \see text
	 */
	virtual void setText(const char *text) = 0;
	
	/*!
	 * Gets the button's current text
	 * \return Null terminated text that will not exceed 16 bytes
	 * \see setText
	 */
	virtual const char *text() const = 0;
	
	/*!
	 * Determines whether or not the text has been recently changed.
	 * \note This function will only return dirty once per change
	 * \attention User code should not call this method on system buttons.
	 */
	virtual bool isTextDirty() const = 0;
	
	/*!
	 * Resets the button's text to the default for that button.
	 * For example, the A button would be reset to "A"
	 */
	virtual void resetText() = 0;
};

class EXPORT_SYM IdButton : public AbstractTextButton
{
public:
	IdButton(const Button::Type::Id& id, const char *defaultText);
	~IdButton();
	
	virtual void setText(const char *text);
	virtual const char *text() const;
	virtual bool isTextDirty() const;
	virtual void setPressed(bool pressed);
	virtual bool value() const;
	virtual void resetText();
	
private:
	Button::Type::Id m_id;
	char *m_defaultText;
};

/*!
 * \class ExtraButtons
 * \brief Helper methods to manipulate and access the state of the X, Y, and Z buttons.
 * \ingroup button
 */
class EXPORT_SYM ExtraButtons
{
public:
	/*!
	 * Shows the X, Y, and Z buttons.
	 * \see setShown
	 * \see hide
	 */
	static inline void show() { setShown(true); }
	
	/*!
	 * Hides the X, Y, and Z buttons.
	 * \see setShown
	 * \see show
	 */
	static inline void hide() { setShown(false); }
	
	/*!
	 * Sets whether or not the X, Y, and Z buttons are to be displayed.
	 * \see isShown
	 * \see show
	 * \see hide
	 */
	static void setShown(const bool& shown);
	
	/*!
	 * Gets whether or not the X, Y, and Z buttons are currently being displayed.
	 * \return true if X, Y, and Z buttons are displayed, false otherwise.
	 * \note This method may not reflect the actual state of the GUI.
	 */
	static bool isShown();
};

/*!
 * The global button instances
 * \ingroup button
 */
namespace Button
{
	//! The A button instance
	extern IdButton A;
	//! The B button instance
	extern IdButton B;
	//! The C button instance
	extern IdButton C;

	//! The X button instance
	extern IdButton X;
	//! The Y button instance
	extern IdButton Y;
	//! The Z button instance
	extern IdButton Z;
	
	//! The Side button instance
	extern IdButton Side;
	
	// extern AbstractButton side;
	
	//! Pointers to all 6 system buttons
	extern IdButton *const all[7];
}

#endif
