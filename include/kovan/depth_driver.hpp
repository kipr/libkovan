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
 * \file depth_driver.hpp
 * \brief C++ interface for depth driver used by libkipr_link_depth_sensor
 * \author Stefan Zeltner
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _DEPTH_DRIVER_HPP_
#define _DEPTH_DRIVER_HPP_

#include "depth_image.hpp"
#include "depth_resolution.h"

namespace depth
{
  class EXPORT_SYM DepthDriver
  {
  public:
    static DepthDriver &instance();

    virtual ~DepthDriver() {};

    /**
      * Opens the depth driver
      */
    virtual void open() = 0;

    /**
      * Closes the depth driver
      */
    virtual void close() = 0;
    
    /**
     * Checks if the depth driver is currently open
     * \return true if open, false otherwise
     */
    virtual bool isOpen() const = 0;

    /**
      * Get the current depth camera resolution
      *
      * \return The current depth camera resolution
      */
    virtual DepthResolution depthCameraResolution() const = 0;

    /**
      * Set the current depth camera resolution.
      *
      * \param resolution New depth camera resolution
      */
    virtual void setDepthCameraResolution(const DepthResolution resolution) = 0;

    /**
      * Returns a Dept Image object containing the current depth values
      *
      * \note returns a null pointer if there is no depth
      *       image available yet.
      * \return DepthImage object
      */
#ifndef SWIG
    virtual DepthImage *depthImage() const = 0;
#endif
  };
}

#endif
