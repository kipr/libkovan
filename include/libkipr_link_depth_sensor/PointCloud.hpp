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
 * \file PointCloud.hpp
 * \brief The interface for a point cloud
 * \author Stefan Zeltner
 */

#ifndef _POINT_CLOUD_HPP_
#define _POINT_CLOUD_HPP_

#include <functional>

#include <libkipr_link_depth_sensor/Point.hpp>
#include <libkipr_link_depth_sensor/BoundingBox.hpp>

namespace libkipr_link_depth_sensor
{
  class PointCloud
  {
  public:
    typedef std::function<void (Point* p)> Callback;

    virtual ~PointCloud() {}

    /**
     * Adds a point to the point cloud
     *
     * \note: The memory ownership of point is transferred to this point
     *        cloud and the point is deleted once the cloud is deleted
     *
     * \param point Add this point to the cloud
     */
    virtual void addPoint(Point* point) = 0;

    /**
     * Gets a point specified by its depth coordinates
     *
     * \note: The returned pointer will point to an invalid location once the
     *        cloud is deleted
     *
     * \param depth_coord Depth image coordinate of this point
     * \returns The point or a nullptr if no point exists
     */
    virtual Point* getPointAtDepthCoordinate(DepthImageCoordinate depth_coord) = 0;

    /**
     * Gets the number of points in the cloud
     *
     * \returns The number of points
     */
    virtual uint32_t numberOfPoints() = 0;

    /**
     * Gets the bounding box of the point cloud
     *
     * \returns The bounding box
     */
    virtual BoundingBox getBoundingBox() = 0;
    
    /**
     * Calls callback for every point in the cloud
     *
     * \param The callback
     */
    virtual void foreach(Callback callback) = 0;
  };
}

#endif /* _POINT_CLOUD_HPP_ */
