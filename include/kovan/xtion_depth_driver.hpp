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
 * \file xtion_depth_driver.hpp
 * \brief OpenNI2 implementation of the DepthDriver interface
 * \author Stefan Zeltner
 * \copyright KISS Insitute for Practical Robotics
 */

#ifndef _XTION_DEPTH_DRIVER_HPP_
#define _XTION_DEPTH_DRIVER_HPP_

#include "kovan/xtion_depth_image.hpp"
#include "kovan/depth_driver.hpp"

namespace depth
{
  class XtionDepthDriverImpl;
  
  class XtionDepthDriver : public DepthDriver
  {
  public:
    static XtionDepthDriver &instance();

    /**
      * Opens the Xtion depth driver
      */
    virtual void open();

    /**
      * Closes the Xtion depth driver
      */
    virtual void close();
    
    /**
     * Checks if the Xtion depth driver is currently open
     * \return true if open, false otherwise
     */
    virtual bool isOpen() const;

    /**
      * Get the current depth camera resolution
      *
      * \return The current depth camera resolution
      */
    virtual DepthResolution depthCameraResolution() const;

    /**
      * Set the current depth camera resolution.
      *
      * \param resolution New depth camera resolution
      */
    virtual void setDepthCameraResolution(const DepthResolution resolution);

    /**
      * Returns a Dept Image object containing the current depth values
      *
      * \return DepthImage object
      */
    virtual DepthImage *depthImage() const;

    ~XtionDepthDriver();

  private:
    XtionDepthDriverImpl *_impl;

    // XtionDepthDriver is a singleton
    XtionDepthDriver();
    XtionDepthDriver(const XtionDepthDriver &);
    void operator =(const XtionDepthDriver &);
  };
}

#endif
