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
EXPORT_SYM point3 get_world_point(int row, int column);

#ifdef __linux__
static const int INVALID_DEPTH = 2147483647;
#else
static const int INVALID_DEPTH = INT32_MAX;
#endif

/**
 * Returns the minimum depth of a scan line
 *
 * \param row Row index of the depth pixel
 * \return The minimum depth of a scan line or INVALID_DEPTH if no depth
 *         image was saved or if the scan line contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_scan_line_min_mask
 * \see depth_scan_line_max
 * \see depth_scan_line_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scan_line_min(int row);

/**
 * Returns the minimum depth of a scan line
 *
 * This version allows to specify a distance mask.
 * Only depth values between min_distance and max_distance
 * are considered. This helps to mask out too near or too
 * far (background) points.
 *
 * \param min_distance Row index of the depth pixel
 * \param max_distance Row index of the depth pixel
 * \param row Row index of the depth pixel
 * \return The minimum depth of a scan line or INVALID_DEPTH if no depth
 *         image was saved or if the scan line contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_scan_line_min
 * \see depth_scan_line_max
 * \see depth_scan_line_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scan_line_min_mask(int row, int min_distance, int max_distance);

/**
 * Returns the maximum depth of a scan line
 *
 * \param row Row index of the depth pixel
 * \return The maximum depth of a scan line or INVALID_DEPTH if no depth
 *         image was saved or if the scan line contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_scan_line_max_mask
 * \see depth_scan_line_min
 * \see depth_scan_line_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scan_line_max(int row);

/**
 * Returns the maximum depth of a scan line
 *
 * This version allows to specify a distance mask.
 * Only depth values between min_distance and max_distance
 * are considered. This helps to mask out too near or too
 * far (background) points.
 *
 * \param min_distance Row index of the depth pixel
 * \param max_distance Row index of the depth pixel
 * \param row Row index of the depth pixel
 * \return The maximum depth of a scan line or INVALID_DEPTH if no depth
 *         image was saved or if the scan line contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_scan_line_min
 * \see depth_scan_line_max
 * \see depth_scan_line_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scan_line_max_mask(int row, int min_distance, int max_distance);

/**
 * Returns the mean depth of a scan line
 *
 * \param row Row index of the depth pixel
 * \return The mean depth of a scan line or INVALID_DEPTH if no depth
 *         image was saved or if the scan line contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_scan_line_max
 * \see depth_scan_line_min
 * \see depth_scan_line_mean_mask
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scan_line_mean(int row);

/**
 * Returns the mean depth of a scan line
 *
 * This version allows to specify a distance mask.
 * Only depth values between min_distance and max_distance
 * are considered. This helps to mask out too near or too
 * far (background) points.
 *
 * \param min_distance Row index of the depth pixel
 * \param max_distance Row index of the depth pixel
 * \param row Row index of the depth pixel
 * \return The mean depth of a scan line or INVALID_DEPTH if no depth
 *         image was saved or if the scan line contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_scan_line_min
 * \see depth_scan_line_max
 * \see depth_scan_line_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_scan_line_mean_mask(int row, int min_distance, int max_distance);

/**
 * Returns the minimum depth of a bounding box
 *
 * \param row_from Bounding box starting from this row index 
 * \param row_to Bounding box until this row index 
 * \param column_from Bounding box until this column index
 * \param column_to Bounding box starting from this column index
 * \return The minimum depth of a bounding box or INVALID_DEPTH if no depth
 *         image was saved or if the bounding box contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_bounding_box_min_mask
 * \see depth_bounding_box_max
 * \see depth_bounding_box_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_bounding_box_min(int row_from, int row_to,
                                      int column_from, int column_to);

/**
 * Returns the minimum depth of a bounding box
 *
 * This version allows to specify a distance mask.
 * Only depth values between min_distance and max_distance
 * are considered. This helps to mask out too near or too
 * far (background) points.
 *
 * \param min_distance Row index of the depth pixel
 * \param max_distance Row index of the depth pixel
 * \param row_from Bounding box starting from this row index 
 * \param row_to Bounding box until this row index 
 * \param column_from Bounding box until this column index
 * \param column_to Bounding box starting from this column index
 * \return The minimum depth of a bounding box or INVALID_DEPTH if no depth
 *         image was saved or if the bounding box contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_bounding_box_min
 * \see depth_bounding_box_max
 * \see depth_bounding_box_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_bounding_box_min_mask(int row_from, int row_to,
                                           int column_from, int column_to,
                                           int min_distance, int max_distance);

/**
 * Returns the maximum depth of a bounding box
 *
 * \param row_from Bounding box starting from this row index 
 * \param row_to Bounding box until this row index 
 * \param column_from Bounding box until this column index
 * \param column_to Bounding box starting from this column index
 * \return The minimum depth of a bounding box or INVALID_DEPTH if no depth
 *         image was saved or if the bounding box contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_bounding_box_max_mask
 * \see depth_bounding_box_min
 * \see depth_bounding_box_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_bounding_box_max(int row_from, int row_to,
                                      int column_from, int column_to);

/**
 * Returns the maximum depth of a bounding box
 *
 * This version allows to specify a distance mask.
 * Only depth values between min_distance and max_distance
 * are considered. This helps to mask out too near or too
 * far (background) points.
 *
 * \param min_distance Row index of the depth pixel
 * \param max_distance Row index of the depth pixel
 * \param row_from Bounding box starting from this row index 
 * \param row_to Bounding box until this row index 
 * \param column_from Bounding box until this column index
 * \param column_to Bounding box starting from this column index
 * \return The minimum depth of a bounding box or INVALID_DEPTH if no depth
 *         image was saved or if the bounding box contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_bounding_box_min
 * \see depth_bounding_box_max
 * \see depth_bounding_box_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_bounding_box_max_mask(int row_from, int row_to,
                                           int column_from, int column_to,
                                           int min_distance, int max_distance);

/**
 * Returns the mean depth of a bounding box
 *
 * \param row_from Bounding box starting from this row index 
 * \param row_to Bounding box until this row index 
 * \param column_from Bounding box until this column index
 * \param column_to Bounding box starting from this column index
 * \return The minimum depth of a bounding box or INVALID_DEPTH if no depth
 *         image was saved or if the bounding box contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_bounding_box_max
 * \see depth_bounding_box_min
 * \see depth_bounding_box_mean_mask
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_bounding_box_mean(int row_from, int row_to,
                                       int column_from, int column_to);

/**
 * Returns the mean depth of a bounding box
 *
 * This version allows to specify a distance mask.
 * Only depth values between min_distance and max_distance
 * are considered. This helps to mask out too near or too
 * far (background) points.
 *
 * \param min_distance Row index of the depth pixel
 * \param max_distance Row index of the depth pixel
 * \param row_from Bounding box starting from this row index 
 * \param row_to Bounding box until this row index 
 * \param column_from Bounding box until this column index
 * \param column_to Bounding box starting from this column index
 * \return The minimum depth of a bounding box or INVALID_DEPTH if no depth
 *         image was saved or if the bounding box contains no visible
 *         depth values
 *
 * \note the row index starts with 0
 *
 * \see depth_bounding_box_min
 * \see depth_bounding_box_max
 * \see depth_bounding_box_mean
 *
 * \ingroup depth
 */
EXPORT_SYM int depth_bounding_box_mean_mask(int row_from, int row_to,
                                            int column_from, int column_to,
                                            int min_distance, int max_distance);

#ifdef __cplusplus
}
#endif

#endif /* _DEPTH_H_ */
