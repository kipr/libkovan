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
 * \file PointCloud2D.hpp
 * \brief A point cloud using an simple 2D array as datastructure
 * \author Stefan Zeltner
 */

#ifndef _POINT_CLOUD_2D_HPP_
#define _POINT_CLOUD_2D_HPP_

#include <vector>

#include "libkipr_link_depth_sensor/PointCloud.hpp"
#include "libkipr_link_depth_sensor/DepthImageSize.hpp"

namespace libkipr_link_depth_sensor
{
  class PointCloud2D : public PointCloud
  {
  public:
    /**
     * Creates an empty point cloud
	   *
	   * \param depth_image_size The size of the depth image in pixel
     */
    PointCloud2D(DepthImageSize depth_image_size);

    /* For memory handling */
    PointCloud2D(const PointCloud2D& other);
    PointCloud2D(PointCloud2D&& other);
    ~PointCloud2D();
    PointCloud2D& operator= (const PointCloud2D& other);
    PointCloud2D& operator= (PointCloud2D&& other);
    
    /**
     * Adds a point to the point cloud
     *
     * \note: The memory ownership of point is transferred to this point
     *        cloud and the point is deleted once the cloud is deleted
     *
     * \param point Add this point to the cloud
     */
    virtual void addPoint(Point* point);
    
    /**
     * Gets a point specified by its depth coordinates
     *
     * \note: The returned pointer will point to an invalid location once the
     *        cloud is deleted
     *
     * \param depth_coord Depth image coordinate of this point
     * \returns The point or a nullptr if no point exists
     */
    virtual Point* getPointAtDepthCoordinate(DepthImageCoordinate depth_coord);

    /**
     * Gets the bounding box of the point cloud
     *
     * \returns The bounding box
     */
    virtual BoundingBox getBoundingBox();

    /**
     * Gets the number of points in the cloud
     *
     * \returns The number of points
     */
    virtual uint32_t numberOfPoints();
    
    /**
     * Calls callback for every point in the cloud
     *
     * \param The callback
     */
    virtual void foreach(Callback callback);
    
  private:
    std::vector<Point*> points_2d_;
    uint32_t n_points_;
    DepthImageSize depth_image_size_;
    BoundingBox bounding_box_;
  };
}

#endif /* _POINT_CLOUD_2D_HPP_ */
