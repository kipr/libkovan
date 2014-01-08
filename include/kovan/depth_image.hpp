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
 * \file depth_image.hpp
 * \brief The interface for a depth image
 * \author Stefan Zeltner
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _DEPTH_IMAGE_HPP_
#define _DEPTH_IMAGE_HPP_

#include <stdint.h>
#include "geom.hpp"

namespace depth
{
  class DepthImage
  {
  public:
    virtual ~DepthImage();

    /**
     * Sets the depth camera orientation
     *
     * \param orientation Depth camera orientation (0 - 359 degree)
     */
    virtual void setOrientation(const uint16_t orientation) = 0;

    /**
     * Gets the depth camera orientation
     *
     * \return Depth camera orientation (0 or 180 degree)
     */
    virtual uint16_t orientation() const = 0;
    
    /**
     * Returns the height of the depth image in pixel
     *
     * \return Height of the depth image in pixel
     */
    virtual uint32_t height() const = 0;

    /**
     * Returns the width of the depth image in pixel
     *
     * \return Width of the depth image in pixel
     */
    virtual uint32_t width() const = 0;

    /**
     * Returns the depth value of the specified point.
     *
     * \param row The row index of the point
     * \param column The column index of the point
     * \return The depth value
     */
    virtual uint16_t depthAt(const uint32_t row, const uint32_t column) const = 0;
    
    
    virtual void depth(uint16_t *const data, const uint32_t offset, const uint32_t size) const = 0;

    /**
     * Returns the specified point.
     *
     * \param row The row index of the point
     * \param column The column index of the point
     * \return The point or 0 if there no point at this coordinate
     */
    virtual Point3<int32_t> pointAt(const uint32_t row, const uint32_t column) const = 0;
  };
}

#endif
