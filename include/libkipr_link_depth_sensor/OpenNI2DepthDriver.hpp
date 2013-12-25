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
 * \file OpenNI2DepthDriver.hpp
 * \brief OpenNI2 implementation of the DepthDriver interface
 * \author Stefan Zeltner
 */

#ifndef _OPENNI2_DEPTH_DRIVER_HPP_
#define _OPENNI2_DEPTH_DRIVER_HPP_


#include <OpenNI.h>

#include "libkipr_link_depth_sensor/OpenNI2DepthImage.hpp"
#include "libkipr_link_depth_sensor/DepthDriver.hpp"

namespace libkipr_link_depth_sensor
{
  class OpenNI2DepthDriver : public DepthDriver,
                             public openni::OpenNI::DeviceConnectedListener,
                             public openni::OpenNI::DeviceDisconnectedListener,
                             public openni::OpenNI::DeviceStateChangedListener,
                             public openni::VideoStream::NewFrameListener
  {
  public:
    static OpenNI2DepthDriver& instance();

    /**
      * Opens the OpenNI2 depth driver
      */
    virtual void open();

    /**
      * Closes the OpenNI2 depth driver
      */
    virtual void close();

    /**
      * Get the current depth camera resolution
      *
      * \return The current depth camera resolution
      */
    virtual DepthCameraResolution getDepthCameraResolution() const;

    /**
      * Set the current depth camera resolution.
      *
      * \param resolution New depth camera resolution
      */
    virtual void setDepthCameraResolution(DepthCameraResolution resolution);

    /**
      * Returns a Dept Image object containing the current depth values
      *
      * \return DepthImage object
      */
    virtual std::shared_ptr<DepthImage> getDepthImage();

    ~OpenNI2DepthDriver();

  private:
    openni::Device device_;
    openni::VideoStream depth_stream_;
    
    std::shared_ptr<OpenNI2DepthImage> last_captured_depth_image_;

    // OpenNI2DepthDriver is a singleton
    OpenNI2DepthDriver();
    OpenNI2DepthDriver(OpenNI2DepthDriver const&);
    void operator=(OpenNI2DepthDriver const&);

    // Implement OpenNI::DeviceConnectedListener::onDeviceConnected()
    virtual void onDeviceConnected(const openni::DeviceInfo* pInfo);

    // Implement OpenNI::DeviceDisconnectedListener::onDeviceDisconnected()
    virtual void onDeviceDisconnected(const openni::DeviceInfo* pInfo);

    // Implement OpenNI::DeviceStateChangedListener::onDeviceStateChanged()
    virtual void onDeviceStateChanged(const openni::DeviceInfo* pInfo, openni::DeviceState state);

    // Implement VideoStream::NewFrameListener::onNewFrame()
    virtual void onNewFrame(openni::VideoStream& stream);
  };
}

#endif /* _OPENNI2_DEPTH_DRIVER_HPP_ */
