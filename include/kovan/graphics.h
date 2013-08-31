#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

/*!
 * \file graphics.h
 * \brief Methods for displaying graphics to the screen.
 * \author Braden McDorman
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup graphics Graphics
 */

#include <kovan/export.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opens a KISS Graphics Window for drawing and input functions.
 * 
 * \param width The width of the graphics window
 * \param height The height of the graphics window
 * \ingroup graphics
 */
EXPORT_SYM int graphics_open(int width, int height);

/**
 * Closes the previously opened KISS Graphics Window
 * \ingroup graphics
 */
EXPORT_SYM void graphics_close();

/**
 * Update must be called after drawing to the window for changes to be visible.
 * This function also must be called regularly for input to work.
 * \ingroup graphics
 */
EXPORT_SYM void graphics_update();

/**
 * Fills the entire window with the color black
 * \ingroup graphics
 */
EXPORT_SYM void graphics_clear();

typedef enum Encoding {
	RGB,
	BGR
} Encoding;


/**
* This function is equivalent to calling graphics_blit_enc() with an encoding of RGB
* \see graphics_blit_enc
*/
EXPORT_SYM void graphics_blit(const unsigned char *data, int x, int y, int width, int height);

/**
 * This function is equivalent to calling graphics_blit_region_enc() with an encoding of RGB
 * \see graphics_blit_region_enc
 */
EXPORT_SYM void graphics_blit_region(const unsigned char *data, int sx, int sy, int ex, int ey, int width, int height, int dx, int dy);

/**
 * Blits a pixel array onto the graphics context
 * 
 * \param data the 1D array of pixel data to blit
 * \param enc RGB for a Red-Green-Blue encoding or BGR for a Blue-Green-Red encoding
 * \param x The destination x coordinate of the blit (target)
 * \param y The destination y coordinate of the blit (target)
 * \param width The width of the area to blit
 * \param height The height of the area to blit
 * \ingroup graphics
 */
EXPORT_SYM void graphics_blit_enc (const unsigned char *data, Encoding enc, int x, int y, int width, int height);

/**
 * Blits a pixel array region onto the graphics context
 * 
 * \param data the 1D array of pixel data to blit
 * \param enc RGB for a Red-Green-Blue encoding or BGR for a Blue-Green-Red encoding
 * \param sx The start x coordinate of the blit (source)
 * \param sy The start y coordinate of the blit (source)
 * \param ex The end x coordinate of the blit (source)
 * \param ey The end y coordinate of the blit (source)
 * \param width The width of the area to blit
 * \param height The height of the area to blit
 * \param dx The destination x coordinate of the blit (target)
 * \param dy The destination y coordinate of the blit (target)
 * \ingroup graphics
 */ 
EXPORT_SYM void graphics_blit_region_enc(const unsigned char *data, Encoding enc, int sx, int sy, int ex, int ey, int width, int height, int dx, int dy);

/**
 * Fill the graphics context with a color.
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_fill(int r, int g, int b);

/**
 * Draw a pixel at (x, y)
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_pixel(int x, int y, int r, int g, int b);

/**
 * Draw a line from the point (x1, y1) to (x2, y2)
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_line(int x1, int y1, int x2, int y2, int r, int g, int b);

/**
 * Draw an unfilled circle to the graphics window at the given coordinates and with the given radius.
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_circle(int cx, int cy, int radius, int r, int g, int b);

/**
 * Draw a filled circle to the graphics window at the given coordinates and with the given radius.
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_circle_fill(int cx, int cy, int radius, int r, int g, int b);

/**
 * Draw an unfilled rectangle to the graphics window at the given coordinates.
 * \param x1 the X coordinate of the rectangle's top-left corner
 * \param y1 the Y coordinate of the rectangle's top-left corner
 * \param x2 the X coordinate of the rectangle's bottom-right corner
 * \param y2 the Y coordinate of the rectangle's bottom-right corner
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_rectangle(int x1, int y1, int x2, int y2, int r, int g, int b);

/**
 * Draw a filled rectangle to the graphics window at the given coordinates.
 * \param x1 the X coordinate of the rectangle's top-left corner
 * \param y1 the Y coordinate of the rectangle's top-left corner
 * \param x2 the X coordinate of the rectangle's bottom-right corner
 * \param y2 the Y coordinate of the rectangle's bottom-right corner
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_rectangle_fill(int x1, int y1, int x2, int y2, int r, int g, int b);

/**
 * Draw an unfilled triangle to the graphics window at the given coordinates.
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_triangle(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b);

/**
 * Draw a filled triangle to the graphics window at the given coordinates.
 * \param r the red component of the drawing, from 0 to 255
 * \param g the green component of the drawing, from 0 to 255
 * \param b the blue component of the drawing, from 0 to 255
 * \ingroup graphics
 */
EXPORT_SYM void graphics_triangle_fill(int x1, int y1, int x2, int y2, int x3, int y3, int r, int g, int b);

enum KeyCode
{
	KeyEnter          = '\n',      ///< enter key
	KeyBackSpace      = '\b',      ///< backspace key
	KeyTab            = '\t',      ///< tab key
	KeyCancel         = 0x03,      ///< cancel key
	KeyClear          = 0x0C,      ///< clear key
	KeyShift          = 0x10,      ///< shift key
	KeyControl        = 0x11,      ///< control key
	KeyAlt            = 0x12,      ///< alt key
	KeyPause          = 0x13,      ///< pause key
	KeyCapsLock       = 0x14,      ///< capslock key
	KeyEscape         = 0x1B,      ///< escape key
	KeySpace          = 0x20,      ///< space key
	KeyPageUp         = 0x21,      ///< page up key
	KeyPageDown       = 0x22,      ///< page down key
	KeyEnd            = 0x23,      ///< end key
	KeyHome           = 0x24,      ///< home key
	KeyLeft           = 0x25,      ///< left key
	KeyUp             = 0x26,      ///< up arrow key
	KeyRight          = 0x27,      ///< right arrow key
	KeyDown           = 0x28,      ///< down arrow key
	KeyComma          = 0x2C,      ///< comma key ','
	KeyPeriod         = 0x2E,      ///< period key '.'
	KeySlash          = 0x2F,      ///< slash key '/'
	KeyZero           = 0x30,      ///< zero key
	KeyOne            = 0x31,      ///< one key
	KeyTwo            = 0x32,      ///< two key
	KeyThree          = 0x33,      ///< three key
	KeyFour           = 0x34,      ///< four key
	KeyFive           = 0x35,      ///< five key
	KeySix            = 0x36,      ///< six key
	KeySeven          = 0x37,      ///< seven key
	KeyEight          = 0x38,      ///< eight key
	KeyNine           = 0x39,      ///< nine key
	KeySemiColon      = 0x3B,      ///< semicolon key ';'
	KeyEquals         = 0x3D,      ///< equals key '='
	KeyA              = 0x41,      ///< a key
	KeyB              = 0x42,      ///< b key
	KeyC              = 0x43,      ///< c key
	KeyD              = 0x44,      ///< d key
	KeyE              = 0x45,      ///< e key
	KeyF              = 0x46,      ///< f key
	KeyG              = 0x47,      ///< g key
	KeyH              = 0x48,      ///< h key
	KeyI              = 0x49,      ///< i key
	KeyJ              = 0x4A,      ///< j key
	KeyK              = 0x4B,      ///< k key
	KeyL              = 0x4C,      ///< l key
	KeyM              = 0x4D,      ///< m key
	KeyN              = 0x4E,      ///< n key
	KeyO              = 0x4F,      ///< o key
	KeyP              = 0x50,      ///< p key
	KeyQ              = 0x51,      ///< q key
	KeyR              = 0x52,      ///< r key
	KeyS              = 0x53,      ///< s key
	KeyT              = 0x54,      ///< t key
	KeyU              = 0x55,      ///< u key
	KeyV              = 0x56,      ///< v key
	KeyW              = 0x57,      ///< w key
	KeyX              = 0x58,      ///< x key
	KeyY              = 0x59,      ///< y key
	KeyZ              = 0x5A,      ///< z key
	KeyOpenBracket    = 0x5B,      ///< open bracket key '['
	KeyBackSlash      = 0x5C,      ///< back slash key '\'
	KeyCloseBracket   = 0x5D,      ///< close bracket key ']'
	KeyNumPad0        = 0x60,      ///< numpad 0 key
	KeyNumPad1        = 0x61,      ///< numpad 1 key
	KeyNumPad2        = 0x62,      ///< numpad 2 key
	KeyNumPad3        = 0x63,      ///< numpad 3 key
	KeyNumPad4        = 0x64,      ///< numpad 4 key
	KeyNumPad5        = 0x65,      ///< numpad 5 key
	KeyNumPad6        = 0x66,      ///< numpad 6 key
	KeyNumPad7        = 0x67,      ///< numpad 7 key
	KeyNumPad8        = 0x68,      ///< numpad 8 key
	KeyNumPad9        = 0x69,      ///< numpad 9 key
	KeyMultiply       = 0x6A,      ///< multiply key '*'
	KeyAdd            = 0x6B,      ///< add key '+'
	KeySeparator      = 0x6C,      ///< separator key '-'
	KeySubtract       = 0x6D,      ///< subtract key '-'
	KeyDecimal        = 0x6E,      ///< decimal key '.'
	KeyDivide         = 0x6F,      ///< divide key '/'
	KeyF1             = 0x70,      ///< F1 key
	KeyF2             = 0x71,      ///< F2 key
	KeyF3             = 0x72,      ///< F3 key
	KeyF4             = 0x73,      ///< F4 key
	KeyF5             = 0x74,      ///< F5 key
	KeyF6             = 0x75,      ///< F6 key
	KeyF7             = 0x76,      ///< F7 key
	KeyF8             = 0x77,      ///< F8 key
	KeyF9             = 0x78,      ///< F9 key
	KeyF10            = 0x79,      ///< F10 key
	KeyF11            = 0x7A,      ///< F11 key
	KeyF12            = 0x7B,      ///< F12 key
	KeyDelete         = 0x7F,      ///< delete key
	KeyNumLock        = 0x90,      ///< numlock key
	KeyScrollLock     = 0x91,      ///< scroll lock key
	KeyPrintScreen    = 0x9A,      ///< print screen key
	KeyInsert         = 0x9B,      ///< insert key
	KeyHelp           = 0x9C,      ///< help key
	KeyMeta           = 0x9D,      ///< meta key
	KeyBackQuote      = 0xC0,      ///< backquote key
	KeyQuote          = 0xDE,      ///< quote key
	KeyFinal          = 0x18,      ///< final key
	KeyConvert        = 0x1C,      ///< convert key
	KeyNonConvert     = 0x1D,      ///< non convert key
	KeyAccept         = 0x1E,      ///< accept key
	KeyModeChange     = 0x1F,      ///< mode change key
	KeyKana           = 0x15,      ///< kana key
	KeyKanji          = 0x19,      ///< kanji key
	KeyUndefined      = 0x0        ///< undefined key
};

/**
 * Gets the current state of a keyboard key.
 * 
 * \note A KISS Graphics Window must be open for input functions to work.
 * \param key An ASCII key code or a Key value from the KeyCode enum.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_key_state(enum KeyCode key);

/**
 * Returns the mouse's current position relative to the top left corner of the graphics window.
 * \ingroup graphics
 */
EXPORT_SYM void get_mouse_position(int *x, int *y);

/**
 * Returns the state of the mouse's middle button.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_mouse_middle_button();

/**
 * Returns the state of the mouse's left button.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_mouse_left_button();

/**
 * Returns the state of the mouse's right button.
 * \return 1 for pressed, 0 for not pressed
 * \ingroup graphics
 */
EXPORT_SYM int get_mouse_right_button();

#ifdef __cplusplus
}
#endif

#endif
