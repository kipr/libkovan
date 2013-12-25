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
     * \param orientation Depth camera orientation (0 or 180 degree)
     */
    virtual void setOrientation(uint16_t orientation);
    
    /**
     * Gets the depth camera orientation
     *
     * \return Depth camera orientation (0 - 359 degree)
     */
    virtual uint16_t getOrientation() const;
    
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
     * \param row The row index of the point
     * \param row The column index of the point
     * \return The depth value
     */
    virtual int32_t getDepthAt(uint32_t row, uint32_t column) const;

    /**
     * Returns the specified point.
     *
     * \param row The row index of the point
     * \param row The column index of the point
     * \return The point or nullptr if there no point at this coordinate
     */
    virtual Point3<int32_t>* getPointAt(uint32_t row, uint32_t column) const;
  
  private:
    openni::VideoFrameRef video_frame_ref_;
    const openni::VideoStream& stream_;

    DepthImageSize size_;

    uint16_t orientation_;
  };
}

#endif /* _OPENNI2_DEPTH_IMAGE_HPP_ */
