/*!
 * \file button.h
 * \brief Methods for manipulating and using software buttons
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _BUTTON_H_
#define _BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * Updates the A button's text.
 * \param text The text to display. Limit of 16 characters.
 */
void set_a_button_text(const char *text);

/*!
 * Updates the B button's text.
 * \param text The text to display. Limit of 16 characters.
 */
void set_b_button_text(const char *text);

/*!
 * Updates the C button's text.
 * \param text The text to display. Limit of 16 characters.
 */
void set_c_button_text(const char *text);

/*!
 * Updates the X button's text.
 * \param text The text to display. Limit of 16 characters.
 */
void set_x_button_text(const char *text);

/*!
 * Updates the Y button's text.
 * \param text The text to display. Limit of 16 characters.
 */
void set_y_button_text(const char *text);

/*!
 * Updates the Z button's text.
 * \param text The text to display. Limit of 16 characters.
 */
void set_z_button_text(const char *text);

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \note This function will block until the button is no longer pressed.
 * \see a_button_pressed
 */
int a_button();

/*!
 * Gets the B button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \note This function will block until the button is no longer pressed.
 * \see b_button_pressed
 */
int b_button();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \note This function will block until the button is no longer pressed.
 * \see c_button_pressed
 */
int c_button();

/*!
 * Gets the X button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \note This function will block until the button is no longer pressed.
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see x_button_pressed
 */
int x_button();

/*!
 * Gets the Y button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \note This function will block until the button is no longer pressed.
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see y_button_pressed
 */
int y_button();

/*!
 * Gets the Z button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \note This function will block until the button is no longer pressed.
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see z_button_pressed
 */
int z_button();

/*!
 * Gets the A button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see a_button
 */
int a_button_pressed();

/*!
 * Gets the B button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see b_button
 */
int b_button_pressed();

/*!
 * Gets the C button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \see c_button
 */
int c_button_pressed();

/*!
 * Gets the X button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see x_button
 */
int x_button_pressed();

/*!
 * Gets the Y button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see y_button
 */
int y_button_pressed();

/*!
 * Gets the Z button's state (pressed or not pressed.)
 * \return 1 for pressed, 0 for not pressed
 * \attention This button is an extra button. Use set_extra_buttons_visible(int) to show this button.
 * \see z_button
 */
int z_button_pressed();

int any_button();

/*!
 * Shows the X, Y, and Z buttons.
 * \see set_extra_buttons_visible
 */
void extra_buttons_show();

/*!
 * Hides the X, Y, and Z buttons. This is the default.
 * \see set_extra_buttons_visible
 */
void extra_buttons_hide();

/*!
 * Determines whether or not the X, Y, and Z buttons are visible.
 * \see set_extra_buttons_visible
 */
int get_extra_buttons_visible();

/*!
 * Sets whether or not the X, Y, and Z buttons are visible.
 * \see get_extra_buttons_visible
 */
void set_extra_buttons_visible(int visible);

#ifdef __cplusplus
}
#endif

#endif
