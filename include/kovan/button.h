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
 * \file button.h
 * \brief Methods for manipulating and using software buttons
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "export.h"

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Updates the A button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 */
EXPORT_SYM void set_a_button_text(const char *text);

/*!
 * Updates the B button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 */
EXPORT_SYM void set_b_button_text(const char *text);

/*!
 * Updates the C button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 */
EXPORT_SYM void set_c_button_text(const char *text);

/*!
 * Updates the X button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 */
EXPORT_SYM void set_x_button_text(const char *text);

/*!
 * Updates the Y button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 */
EXPORT_SYM void set_y_button_text(const char *text);

/*!
 * Updates the Z button's text.
 * \param text The text to display. Limit of 16 characters.
 * \ingroup button
 */
EXPORT_SYM void set_z_button_text(const char *text);

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button_clicked
 * \ingroup button
 */
EXPORT_SYM int a_button();

/*!
 * Gets the B button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see b_button_clicked
 * \ingroup button
 */
EXPORT_SYM int b_button();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see c_button_clicked
 * \ingroup button
 */
EXPORT_SYM int c_button();

/*!
 * Gets the X button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see x_button_clicked
 * \ingroup button
 */
EXPORT_SYM int x_button();

/*!
 * Gets the Y button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see y_button_clicked
 * \ingroup button
 */
EXPORT_SYM int y_button();

/*!
 * Gets the Z button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see z_button_clicked
 * \ingroup button
 */
EXPORT_SYM int z_button();

/*!
 * Gets the side button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see side_button_clicked
 * \ingroup button
 */
EXPORT_SYM int side_button();

/*!
 * Gets the side button's state (pressed or not pressed.)
 * \deprecated Use side_button() instead.
 * \return 1 for pressed, 0 for not pressed
 * \see side_button
 * \ingroup button
 */
EXPORT_SYM int black_button();

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 * \ingroup button
 */
EXPORT_SYM int a_button_clicked();

/*!
 * Gets the B button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see b_button
 * \ingroup button
 */
EXPORT_SYM int b_button_clicked();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see c_button
 * \ingroup button
 */
EXPORT_SYM int c_button_clicked();

/*!
 * Gets the X button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see x_button
 * \ingroup button
 */
EXPORT_SYM int x_button_clicked();

/*!
 * Gets the Y button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see y_button
 * \ingroup button
 */
EXPORT_SYM int y_button_clicked();

/*!
 * Gets the Z button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see z_button
 * \ingroup button
 */
EXPORT_SYM int z_button_clicked();

/*!
 * Gets the side button's state (pressed or not pressed.)
 * \blocksuntil the button is no longer pressed.
 * \return 1 for pressed, 0 for not pressed
 * \see side_button
 * \ingroup button
 */
EXPORT_SYM int side_button_clicked();

EXPORT_SYM int any_button();

/*!
 * Shows the X, Y, and Z buttons.
 * \see set_extra_buttons_visible
 * \ingroup button
 */
EXPORT_SYM void extra_buttons_show();

/*!
 * Hides the X, Y, and Z buttons. This is the default.
 * \see set_extra_buttons_visible
 * \ingroup button
 */
EXPORT_SYM void extra_buttons_hide();

/*!
 * Determines whether or not the X, Y, and Z buttons are visible.
 * \see set_extra_buttons_visible
 * \ingroup button
 */
EXPORT_SYM int get_extra_buttons_visible();

/*!
 * Sets whether or not the X, Y, and Z buttons are visible.
 * \see get_extra_buttons_visible
 * \ingroup button
 */
EXPORT_SYM void set_extra_buttons_visible(int visible);

#ifdef __cplusplus
}
#endif

#endif
