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
 * \file openni2_depth_image.hpp
 * \brief OpenNI2 implementation of the DepthImage interface
 * \author Stefan Zeltner
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _OPENNI2_DEPTH_IMAGE_HPP_
#define _OPENNI2_DEPTH_IMAGE_HPP_

#include <OpenNI.h>

#include "kovan/depth_image.hpp"
#include "kovan/geom.hpp"

namespace depth
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
     * \param column The column index of the point
     * \return The depth value
     */
    virtual int32_t getDepthAt(uint32_t row, uint32_t column) const;

    /**
     * Returns the specified point.
     *
     * \param row The row index of the point
     * \param column The column index of the point
     * \return The point or 0 if there no point at this coordinate
     */
    virtual Point3<int32_t>* getPointAt(uint32_t row, uint32_t column) const;
  
  private:
    openni::VideoFrameRef video_frame_ref_;
    const openni::VideoStream& stream_;

    Rect<uint32_t> size_;

    uint16_t orientation_;
  };
}

#endif /* _OPENNI2_DEPTH_IMAGE_HPP_ */
