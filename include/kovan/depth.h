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
 * Get the current depth camera resolution
 *
 * \return The current depth camera resolution
 *
 * \see set_depth_camera_resolution
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM DepthResolution get_depth_camera_resolution();

/**
 * Set the current depth camera resolution.
 *
 * \param resolution New depth camera resolution
 * \return 1 on success, 0 otherwise
 *
 * \see get_depth_image_resolution
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM int set_depth_camera_resolution(DepthResolution resolution);

/**
 * Sets the depth camera orientation
 *
 * \param orientation Depth camera orientation
 *
 * \return 1 on success, 0 otherwise
 *
 * \note Only 0 and 180 degree are supported yet
 *
 * \ingroup depth
 */
EXPORT_SYM int set_depth_camera_orientation(int orientation);

/**
 * Gets the depth camera orientation
 *
 * \return Depth camera orientation or 0xFFFF in case of an error
 *
 * \ingroup depth
 */
EXPORT_SYM int get_depth_camera_orientation();

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
EXPORT_SYM int depth_image_get_height();

/**
 * Returns the width of the depth image stored by depth_update in pixel
 *
 * \return Width of the depth image in pixel or 0 if no depth image was saved
 *
 * \see depth_update
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_image_get_width();


static const int INVALID_COORDINATE = INT32_MIN;

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
 * Returns the x coordinate of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The x coordinate in millimeters or INVALID_COORDINATE if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \see get_world_y
 * \see get_world_z
 *
 * \ingroup depth
 */
EXPORT_SYM int get_world_x(int row, int column);

/**
 * Returns the y coordinate of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The y coordinate in millimeters or INVALID_COORDINATE if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \see get_world_x
 * \see get_world_z
 *
 * \ingroup depth
 */
EXPORT_SYM int get_world_y(int row, int column);

/**
 * Returns the z coordinate of the specified point.
 *
 * \param row Row index of the depth pixel
 * \param column Column index of the depth pixel
 * \return The z coordinate in millimeters or INVALID_COORDINATE if no depth
 *         image was saved or if the depth value is not visible
 *         by the depth sensor
 *
 * \note the row/column index starts with 0
 *
 * \see get_world_x
 * \see get_world_y
 *
 * \ingroup depth
 */
EXPORT_SYM int get_world_z(int row, int column);

#ifdef __cplusplus
}
#endif

#endif /* _DEPTH_H_ */
