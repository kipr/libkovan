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

#include "libkipr_link_depth_sensor/Exception.hpp"
#include "libkipr_link_depth_sensor/OpenNI2DepthDriver.hpp"

using namespace libkipr_link_depth_sensor;
using namespace openni;

OpenNI2DepthDriver& OpenNI2DepthDriver::instance()
{
  static OpenNI2DepthDriver _instance;

  return _instance;
}

void OpenNI2DepthDriver::onDeviceConnected(const DeviceInfo* pInfo)
{
  
}

void OpenNI2DepthDriver::onDeviceDisconnected(const DeviceInfo* pInfo)
{
  
}

void OpenNI2DepthDriver::onDeviceStateChanged(const DeviceInfo* pInfo,
                                                DeviceState state)
{
  
}

void OpenNI2DepthDriver::onNewFrame(VideoStream& stream)
{
  VideoFrameRef ref;
  stream.readFrame(&ref);

  last_captured_depth_image_.reset(new OpenNI2DepthImage(ref, depth_stream_));
}

OpenNI2DepthDriver::OpenNI2DepthDriver()
{
  Status rc = OpenNI::initialize();
  if(rc != STATUS_OK)
  {
    throw Exception(std::string("Initialize failed with ")
      + OpenNI::getExtendedError());
  }

  OpenNI::addDeviceConnectedListener(this);
  OpenNI::addDeviceDisconnectedListener(this);
  OpenNI::addDeviceStateChangedListener(this);
}

OpenNI2DepthDriver::~OpenNI2DepthDriver()
{
  close();

  OpenNI::removeDeviceConnectedListener(this);
  OpenNI::removeDeviceDisconnectedListener(this);
  OpenNI::removeDeviceStateChangedListener(this);

  OpenNI::shutdown();
}

void OpenNI2DepthDriver::open()
{
  if(!device_.isValid())
  {
    Status rc = device_.open(ANY_DEVICE);
    if(rc != STATUS_OK)
    {
      throw Exception(std::string("Open the device failed with\n")
        + OpenNI::getExtendedError());
    }
    device_.open(ANY_DEVICE);

    if(device_.getSensorInfo(SENSOR_DEPTH) == NULL)
    {
      device_.close();
      throw Exception("Device has no depth sensor!");
    }

    rc = depth_stream_.create(device_, SENSOR_DEPTH);
    if(rc != STATUS_OK)
    {
      device_.close();
      throw Exception(std::string("Create the depth stream failed with\n")
        + OpenNI::getExtendedError());
    }

    VideoMode video_mode = depth_stream_.getVideoMode();
    video_mode.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);

    rc = depth_stream_.setVideoMode(video_mode);
    if(rc != STATUS_OK)
    {
        throw Exception(std::string("Set the pixel format to "
            "PIXEL_FORMAT_DEPTH_1_MM failed with\n")
            + OpenNI::getExtendedError());
    }

    rc = depth_stream_.start();
    if(rc != STATUS_OK)
    {
      depth_stream_.destroy();
      device_.close();

      throw Exception(std::string("Starting the depth stream failed with\n")
        + OpenNI::getExtendedError());
    }

    rc = depth_stream_.addNewFrameListener(this);
    if(rc != STATUS_OK)
    {
      depth_stream_.stop();
      depth_stream_.destroy();
      device_.close();

      throw Exception(std::string("Adding the frame listener failed with\n")
        + OpenNI::getExtendedError());
    }
  }
}

void OpenNI2DepthDriver::close()
{
  depth_stream_.removeNewFrameListener(this);

  depth_stream_.stop();
  depth_stream_.destroy();

  device_.close();
}

DepthCameraResolution OpenNI2DepthDriver::getDepthCameraResolution() const
{
  VideoMode video_mode = depth_stream_.getVideoMode();

  int res_x = video_mode.getResolutionX();
  int res_y = video_mode.getResolutionY();

  if((res_x == 320) && (res_y = 240))
  {
    return DEPTH_CAMERA_RESOLUTION_320_240;
  }
  else if((res_x == 640) && (res_y = 480))
  {
    return DEPTH_CAMERA_RESOLUTION_640_480;
  }
  else
  {
    return DEPTH_CAMERA_INVALID_RESOLUTION;
  }
}

void OpenNI2DepthDriver::setDepthCameraResolution(DepthCameraResolution resolution)
{
  depth_stream_.stop();
  last_captured_depth_image_.reset();
  
  VideoMode video_mode = depth_stream_.getVideoMode();

  if(resolution == DEPTH_CAMERA_RESOLUTION_320_240)
  {
    video_mode.setResolution(320, 240);
  }
  else if(resolution == DEPTH_CAMERA_RESOLUTION_640_480)
  {
    video_mode.setResolution(640, 480);
  }
  else
  {
    depth_stream_.start();
    throw Exception(std::string("Invalid resolution"));
  }

  Status rc = depth_stream_.setVideoMode(video_mode);
  if (rc != STATUS_OK)
  {
    depth_stream_.start();
    throw Exception(std::string("Set the resolution failed with\n")
      + OpenNI::getExtendedError());
  }
  
  rc = depth_stream_.start();
  if(rc != STATUS_OK)
  {
    // how handle this?!
    close();
    
    throw Exception(std::string("Unable to start the depth stream"));
  }
}

std::shared_ptr<DepthImage> OpenNI2DepthDriver::getDepthImage()
{
  return last_captured_depth_image_;
}
