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
 * \file OpenNI2DepthImage.hpp
 * \brief OpenNI2 implementation of the DepthImage interface
 * \author Stefan Zeltner
 */

#ifndef _OPENNI2_DEPTH_IMAGE_HPP_
#define _OPENNI2_DEPTH_IMAGE_HPP_

#include <OpenNI.h>

#include "libkipr_link_depth_sensor/DepthImage.hpp"
#include "libkipr_link_depth_sensor/DepthImageSize.hpp"

namespace libkipr_link_depth_sensor
{
  class OpenNI2DepthImage : public DepthImage
  {
  public:
    OpenNI2DepthImage(openni::VideoFrameRef video_frame_ref,
                    const openni::VideoStream& stream);

    /**
     * Sets the depth camera orientation
     *
     * \param orientation Depth camera orientation
     */
    void setOrientation(DepthCameraOrientation orientation);

    /**
     * Gets the depth camera orientation
     *
     * \return Depth camera orientation or DEPTH_CAMERA_INVALID_ORIENTATION
     */
    DepthCameraOrientation getOrientation();
    
    /**
    * Returns the height of the depth image in pixel
    *
    * \return Height of the depth image in pixel
    */
    virtual uint32_t getHeight() const;

    /**
    * Returns the width of the depth image in pixel
    *
    * \return Width of the depth image in pixel
    */
    virtual uint32_t getWidth() const;

    /**
     * Returns the depth value of the specified point.
     *
     * \param coordinate The coordinate of the point
     * \return The depth value
     */
    virtual int32_t getDepthAt(const DepthImageCoordinate& coordinate) const;

    /**
     * Returns the specified point.
     *
     * \param coordinate The coordinate of the point
     * \return The point or nullptr if there no point at this coordinate
     */
    virtual Point* getPointAt(const DepthImageCoordinate& coordinate) const;
    
    /**
     * Returns a PointCloud object of this depth image
     *
     * The filter function is used to add only points to the point cloud
     * which meet the filter criteria. In addition it can be used to modify the
     * depth value (e.g. average over the surrounding pixel) before put them
     * into the point cloud. The _this parameter of the filter can be used to
     * get the depth of the surrounding pixel via getDepthAt()
     *
     * \note Creating a point cloud is very expensive. Therefore the filter
     * should filter out as much as possible.
     *
     * \return A PointCloud object
     */
    virtual PointCloud* getPointCloud(Filter filter) const;
  
  private:
    openni::VideoFrameRef video_frame_ref_;
    const openni::VideoStream& stream_;

    DepthImageSize size_;

    DepthCameraOrientation orientation_;
  };
}

#endif /* _OPENNI2_DEPTH_IMAGE_HPP_ */
