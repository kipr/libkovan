/* *****************************************************************************

    This file is part of the depth sensor library for the
        KIPR Link Robot Controller.

    libkipr_link_depth_sensor is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libkipr_link_depth_sensor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libkipr_link_depth_sensor.
    If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************/

/**
 * \file depth.h
 * \brief C API of the KIPR Link depth sensor interface
 * \author Stefan Zeltner
 *
 * This API is keept simple so even unexperienced C programmers are able to use
 * the KIPR Link dept sensor library.
 *
 * Object-oriented programmers should use the C++ API defined in
 * depth.hpp.
 */

#ifndef _DEPTH_H_
#define _DEPTH_H_

#include <stdint.h>

#include <libkipr_link_depth_sensor/DepthCameraResolution.h>
#include <libkipr_link_depth_sensor/DepthCameraOrientation.h>
#include <libkipr_link_depth_sensor/Library.h>

#ifdef __cplusplus
extern "C" {
#endif

/** \name Open/Close
 * Use these functions to open / close the depth sensor library in a clean way.
 */
/** \{ */

/**
 * Opens the depth sensor. This is the first function which has to be called
 * before any other function!
 *
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int depth_open();

/**
 * Closes the depth sensor
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int depth_close();

/** \} */


/** \name Depth camera resolution
 * Use these functions to get and set the resolution of the depth camera.
 * A new depth image pulled by depth_update() has this resolution.
 *
 * \see depth_update
 */
/** \{ */

/**
 * Get the current depth camera resolution
 *
 * \return The current depth camera resolution
 *
 * \see set_depth_camera_resolution
 * \see depth_update
 */
LIBRARY_API DepthCameraResolution get_depth_camera_resolution();

/**
 * Set the current depth camera resolution.
 *
 * \param resolution New depth camera resolution
 * \return 1 on success, 0 otherwise
 *
 * \see get_depth_image_resolution
 * \see depth_update
 */
LIBRARY_API int set_depth_camera_resolution(DepthCameraResolution resolution);

/** \} */


/** \name Camera orientation
 */
/** \{ */

/**
 * Sets the depth camera orientation
 *
 * \param orientation Depth camera orientation
 *
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int set_depth_camera_orientation(DepthCameraOrientation orientation);

/**
 * Gets the depth camera orientation
 *
 * \return Depth camera orientation or DEPTH_CAMERA_INVALID_ORIENTATION
 */
LIBRARY_API DepthCameraOrientation get_depth_camera_orientation();

/** \} */



/** \name Pull a depth image
 */
/** \{ */

/**
 * Pulls a new depth image from the depth sensor for future processing.
 *
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int depth_update();

/** \} */



/** \name Access raw depth measurements
 * Use these functions to retrieve raw depth measurements of the saved depth image.
 * You have to call depth_update() before you can access the raw depth
 * measurements.
 *
 * \note All the raw depth functions use a different coordinate system than
 *       the point cloud functions:
 *       x and y are used to address a depth value within of the depth image.
 *       The range of x is between 0 and depth_image_get_width()-1, the range of
 *       y is between 0 and depth_image_get_height()-1.
 *       (0, 0) address the depth value in the upper left corner.
 *
 * \see depth_update
 */
/** \{ */

/**
 * Returns the height of the depth image stored by depth_update in pixel
 *
 * \see depth_update
 *
 * \return Height of the depth image in pixel or 0 if no depth image was saved
 */
LIBRARY_API int depth_image_get_height();

/**
 * Returns the width of the depth image stored by depth_update in pixel
 *
 * \see depth_update
 *
 * \return Width of the depth image in pixel or 0 if no depth image was saved
 */
LIBRARY_API int depth_image_get_width();


static const int INVALID_COORDINATE = INT32_MIN;

/**
 * Returns the depth value of the specified point.
 *
 * \see depth_image_get_height
 * \see depth_image_get_width
 *
 * \param x X coordinate of the depth value (depth coordinate system)
 * \param y Y coordinate of the depth value (depth coordinate system)
 * \return The depth value in millimeters or INVALID_COORDINATE if no depth
 *         image was saved or if the depth value of (x, y) is not visible
 *         by the depth sensor
 */
LIBRARY_API int get_depth_value(int x, int y);

/**
 * Returns the x coordinate of the specified point.
 *
 * \see get_world_y
 * \see get_world_z
 *
 * \param x X coordinate of the point (depth coordinate system)
 * \param y Y coordinate of the point (depth coordinate system)
 * \return The x value (world coordinate system) in millimeters or INVALID_COORDINATE
*          if no depth image was saved or if the depth value of (x, y) is not visible
*          by the depth sensor
 */
LIBRARY_API int get_world_x(int x, int y);

/**
 * Returns the y coordinate of the specified point.
 *
 * \see get_world_x
 * \see get_world_z
 *
 * \param x X coordinate of the point (depth coordinate system)
 * \param y Y coordinate of the point (depth coordinate system)
 * \return The y value (world coordinate system) in millimeters or INVALID_COORDINATE
*          if no depth image was saved or if the depth value of (x, y) is not visible
*          by the depth sensor
 */
LIBRARY_API int get_world_y(int x, int y);

/**
 * Returns the z coordinate of the specified point.
 *
 * \see get_world_x
 * \see get_world_y
 *
 * \param x X coordinate of the point (depth coordinate system)
 * \param y Y coordinate of the point (depth coordinate system)
 * \return The z value (world coordinate system) in millimeters or INVALID_COORDINATE
*          if no depth image was saved or if the depth value of (x, y) is not visible
*          by the depth sensor
 */
LIBRARY_API int get_world_z(int x, int y);

/** \} */



/** \name Point cloud functions
 * Use these functions to create a point cloud from a specific depth image region.
 * calling point_cloud_update() is required before any other function can be
 * called.
 *
 * \note All the point cloud functions use a different coordinate system than
 *       the raw depth functions:
 *       x, y and z are used to define a point in a 3D space. The unit is mm.
 *       From camera perspective, the y coordinate increases if an object moves
 *       upwards, the x coordinate increases if it moves from left to right and
 *       the z coordinate increase if it moves straight away from the camera.
 *
 * \see point_cloud_update
 */
/** \{ */

typedef enum PointCloudUpdateFilter_
{
  POINT_CLOUD_FILTER_MIN_X,
  POINT_CLOUD_FILTER_MAX_X,
  POINT_CLOUD_FILTER_MIN_Y,
  POINT_CLOUD_FILTER_MAX_Y,
  POINT_CLOUD_FILTER_MIN_DEPTH,
  POINT_CLOUD_FILTER_MAX_DEPTH,
} PointCloudUpdateFilter;

/**
 * Add a point cloud update filter
 *
 * Add filter to restrict the number of points generated by point_cloud_update().
 * The filter takes effect the next time point_cloud_update() is called.
 * You can add multiple filter like one to restrict the minimum and one to select
 * the maximum depth.
 *
 * \see point_cloud_update
 * \see reset_point_cloud_update_filter
 *
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int add_point_cloud_update_filter(PointCloudUpdateFilter filter, int value);

/**
 * Remove all point cloud update filter
 *
 * \see point_cloud_update
 * \see add_point_cloud_update_filter
 *
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int reset_point_cloud_update_filter();

/**
 * Save the current point cloud for future processing.
 *
 * \note It is highly recommended to apply a filter before calling
 *       point_cloud_update() as the performance decrease if too many points
 *       are selected.
 *
 * \see add_point_cloud_update_filter
 * \see reset_point_cloud_update_filter
 *
 * \return 1 on success, 0 otherwise
 */
LIBRARY_API int point_cloud_update();

/**
  * Gets the number of points in the cloud
  *
  * \returns The number of points
  */
LIBRARY_API int points_in_cloud();

static const int INVALID_MIN = INT32_MIN;
static const int INVALID_MAX = INT32_MAX;

/**
 * Get the point cloud bounding box
 *
 * \see get_cloud_min_x
 * \see get_cloud_min_z
 * \see get_cloud_max_x
 * \see get_cloud_max_y
 * \see get_cloud_max_z
 *
 * \return The minimum x coordinate of all points or INVALID_MIN
 *         if an error occurs
 */
LIBRARY_API int get_cloud_min_x();

/**
 * Get the point cloud bounding box
 *
 * \see get_cloud_min_y
 * \see get_cloud_min_z
 * \see get_cloud_max_x
 * \see get_cloud_max_y
 * \see get_cloud_max_z
 *
 * \return The minimum y coordinate of all points or INVALID_MIN
 *         if an error occurs
 */
LIBRARY_API int get_cloud_min_y();

/**
 * Get the point cloud bounding box
 *
 * \see get_cloud_min_x
 * \see get_cloud_min_y
 * \see get_cloud_max_x
 * \see get_cloud_max_y
 * \see get_cloud_max_z
 *
 * \return The minimum z coordinate of all points or INVALID_MIN
 *         if an error occurs
 */
LIBRARY_API int get_cloud_min_z();

/**
 * Get the point cloud bounding box
 *
 * \see get_cloud_min_x
 * \see get_cloud_min_y
 * \see get_cloud_min_z
 * \see get_cloud_max_y
 * \see get_cloud_max_z
 *
 * \return The maximum x coordinate of all points or INVALID_MAX
 *         if an error occurs
 */
LIBRARY_API int get_cloud_max_x();

/**
 * Get the point cloud bounding box
 *
 * \see get_cloud_min_x
 * \see get_cloud_min_y
 * \see get_cloud_min_z
 * \see get_cloud_max_x
 * \see get_cloud_max_z
 *
 * \return The maximum y coordinate of all points or INVALID_MAX
 *         if an error occurs
 */
LIBRARY_API int get_cloud_max_y();

/**
 * Get the point cloud bounding box
 *
 * \see get_cloud_min_x
 * \see get_cloud_min_y
 * \see get_cloud_min_z
 * \see get_cloud_max_x
 * \see get_cloud_max_y
 *
 * \return The maximum z coordinate of all points or INVALID_MAX
 *         if an error occurs
 */
LIBRARY_API int get_cloud_max_z();


typedef enum PointCloudColorMode_
{
  POINT_CLOUD_COLOR_MODE_GREY_SCALE,
  POINT_CLOUD_COLOR_MODE_HUE_GRADIENT,
  POINT_CLOUD_COLOR_MODE_RGB_GRADIENT
} PointCloudColorMode;

/**
  * Color the points of the cloud
  *
  * \param mode How to color the cloud
  *
  * \return 1 on success, 0 otherwise
  */
LIBRARY_API int color_cloud(PointCloudColorMode mode);

/**
  * Gets a point color from the point cloud specified by its depth coordinates
  *
  * \param depth_x X coordinate of this point
  * \param depth_y Y coordinate of this point
  *
  * \returns The red color value of this point
  */
LIBRARY_API int get_point_color_red(int depth_x, int depth_y);

/**
  * Gets a point color from the point cloud specified by its depth coordinates
  *
  * \param depth_x X coordinate of this point
  * \param depth_y Y coordinate of this point
  *
  * \returns The green color value of this point
  */
LIBRARY_API int get_point_color_green(int depth_x, int depth_y);

/**
  * Gets a point color from the point cloud specified by its depth coordinates
  *
  * \param depth_x X coordinate of this point
  * \param depth_y Y coordinate of this point
  *
  * \returns The blue color value of this point
  */
LIBRARY_API int get_point_color_blue(int depth_x, int depth_y);

/** \} */

LIBRARY_API int get_conected_component_rgb(int x, int y, int* r, int* g, int* b);
LIBRARY_API int update_conected_component(int delta);

#ifdef __cplusplus
}
#endif

#endif /* _DEPTH_H_ */
