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

/**
 * \file depth.h
 * \brief C API of the KIPR Link depth sensor interface
 * \author Stefan Zeltner
 * \copyright KISS Insitute for Practical Robotics
 * \defgroup depth Depth Sensor
 */

#ifndef _DEPTH_H_
#define _DEPTH_H_

#include <stdint.h>

#include <kovan/depth_resolution.h>
#include <kovan/export.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Opens the depth sensor. This is the first function which has to be called
 * before any other function!
 *
 * \return 1 on success, 0 otherwise
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_open();

/**
 * Closes the depth sensor
 * \return 1 on success, 0 otherwise
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_close();

/**
 * Get the current depth image resolution
 *
 * \return The current depth image resolution
 *
 * \see set_depth_resolution
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM DepthResolution get_depth_resolution();

/**
 * Set the current depth image resolution.
 *
 * \param resolution New depth image resolution
 * \return 1 on success, 0 otherwise
 *
 * \see get_depth_resolution
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM int set_depth_resolution(DepthResolution resolution);

/**
 * Sets the depth image orientation
 *
 * \param orientation Depth image orientation
 *
 * \return 1 on success, 0 otherwise
 *
 * \note Only 0 and 180 degree are supported yet
 *
 * \ingroup depth
 */
EXPORT_SYM int set_depth_orientation(int orientation);

/**
 * Gets the depth image orientation
 *
 * \return Depth image orientation or 0xFFFF in case of an error
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_orientation();

/**
 * Pulls a new depth image from the depth sensor for future processing.
 *
 * \return 1 on success, 0 otherwise
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_update();

/**
 * Returns the height of the depth image stored by depth_update in pixel
 *
 * \return Height of the depth image in pixel or 0 if no depth image was saved
 *
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_image_height();

/**
 * Returns the width of the depth image stored by depth_update in pixel
 *
 * \return Width of the depth image in pixel or 0 if no depth image was saved
 *
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_image_width();

/**
 * Returns the depth value of a depth pixel.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The depth value in millimeters or INVALID_COORDINATE if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \see depth_image_get_height
 * \see depth_image_get_width
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_value(int row, int column);

/**
 * Returns the world coordinates of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The world coordinates in millimeters or (-1, -1, -1) if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \ingroup depth
 */
EXPORT_SYM point3 get_depth_world_point(int row, int column);

/**
 * Returns the x world coordinate of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The x world coordinate in millimeters or -1 if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_world_point_x(int row, int column);

/**
 * Returns the y world coordinate of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The y world coordinate in millimeters or -1 if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_world_point_y(int row, int column);

/**
 * Returns the z world coordinate of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The z world coordinate in millimeters or -1 if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_world_point_z(int row, int column);

#ifdef __linux__
static const int INVALID_DEPTH = 2147483647;
#else
static const int INVALID_DEPTH = INT32_MAX;
#endif


/**
 * Selects a row for further processing by get_depth_scanline functions.
 *
 * \return 1 on success, 0 otherwise
 *
 * \note the row/column index starts with 0
 * \note Calls the depth_update() will invalidate the scanline selection.
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scanline_update(int row);

/**
 * Retrieve the number of objects detected on the selected scanline.
 *
 * \return The number of objects detected, or -1 on error
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_count();

EXPORT_SYM point3 get_depth_scanline_object_point(int object_num);

/**
 * Retrieve the given object's center x offset from the camera.
 *
 * \return The x offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_center_x(int object_num);

/**
 * Retrieve the given object's center y offset from the camera.
 *
 * \return The object's center y offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_center_y(int object_num);

/**
 * Retrieve the given object's center z offset from the camera.
 *
 * \return The object's center z offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_center_z(int object_num);

/**
 * Retrieve the given object's nearest x offset from the camera.
 *
 * \return The object's nearest x offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_nearest_x(int object_num);

/**
 * Retrieve the given object's nearest y offset from the camera.
 *
 * \return The object's nearest y offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_nearest_y(int object_num);

/**
 * Retrieve the given object's nearest z offset from the camera.
 *
 * \return The object's nearest z offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_nearest_z(int object_num);

/**
 * Retrieve the given object's farthest x offset from the camera.
 *
 * \return The object's farthest x offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_farthest_x(int object_num);

/**
 * Retrieve the given object's farthest y offset from the camera.
 *
 * \return The object's farthest y offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_farthest_y(int object_num);

/**
 * Retrieve the given object's farthest z offset from the camera.
 *
 * \return The object's farthest z offset from the center of the Xtion sensor, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_farthest_z(int object_num);

/**
 * Retrieve the given object's size in mm.
 *
 * \return The object's size in mm, or -1 on error.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_size(int object_num);

/**
 * Retrieve the given object's angle in degrees.
 *
 * \return The object's angle from the camera, or -1 on error.
 *
 * \note An object angle of 0 degrees is parallel to the camera.
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_scanline_object_angle(int object_num);

typedef enum SortMethod_
{
  SORT_NEAREST = 0,
  SORT_CENTER,
  SORT_FARTHEST
} SortMethod;

EXPORT_SYM void set_depth_scanline_sorting_method(SortMethod method);
EXPORT_SYM SortMethod get_depth_scanline_sorting_method();

#ifdef __cplusplus
}
#endif

#endif /* _DEPTH_H_ */
