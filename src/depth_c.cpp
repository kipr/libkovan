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

#include <iostream>
#include <exception>
#include <memory>

#include "libkipr_link_depth_sensor/Exception.hpp"
#include "libkipr_link_depth_sensor/DepthDriver.hpp"
#include "libkipr_link_depth_sensor/Point.hpp"
#include "libkipr_link_depth_sensor/depth.h"

namespace libkipr_link_depth_sensor
{
  namespace c_api
  {
    std::shared_ptr<DepthImage> _depth_image;
    DepthCameraOrientation _orientation = DEPTH_CAMERA_ORIENTATION_DEFAULT;
    
    struct MinMaxFilterValue
    {
    private:
      bool min_is_set;
      int min;
      bool max_is_set;
      int max;
      
    public:
      MinMaxFilterValue()
        : min_is_set(false), max_is_set(false) {}
      
      void setMin(int value)
      {
        min_is_set = true;
        min = value;
      }
      
      void setMax(int value)
      {
        max_is_set = true;
        max = value;
      }
      
      bool filter(int value)
      {
        if(min_is_set && (min > value))
        {
          return false;
        }
        if(max_is_set && (max < value))
        {
          return false;
        }
        
        return true;
      }
    } _filter_x, _filter_y, _filter_depth;
  }
}

using namespace libkipr_link_depth_sensor;
using namespace libkipr_link_depth_sensor::c_api;

#define catchAllAndReturn(return_value) \
  catch(std::exception& e) { std::cerr << e.what() << std::endl; } \
  catch(const char* msg) { std::cerr << msg << std::endl; } \
  catch(...) {} \
  return (return_value)

int depth_open()
{
  try
  {
    DepthDriver::instance().open();

    return 1;
  }
  catchAllAndReturn(0);
}

int depth_close()
{
  try
  {
    DepthDriver::instance().close();

    return 1;
  }
  catchAllAndReturn(0);
}

DepthCameraResolution get_depth_camera_resolution()
{
  try
  {
    return DepthDriver::instance().getDepthCameraResolution();
  }
  catchAllAndReturn(DEPTH_CAMERA_INVALID_RESOLUTION);
}

int set_depth_camera_resolution(DepthCameraResolution resolution)
{
  try
  {
    DepthDriver::instance().setDepthCameraResolution(resolution);

    return 1;
  }
  catchAllAndReturn(0);
}

int set_depth_camera_orientation(DepthCameraOrientation orientation)
{
  try
  {
    _orientation = orientation;

    return 1;
  }
  catchAllAndReturn(0);
}

DepthCameraOrientation get_depth_camera_orientation()
{
  try
  {
    return _orientation;
  }
  catchAllAndReturn(DEPTH_CAMERA_INVALID_ORIENTATION);
}

int depth_update()
{
  try
  {
    _depth_image = DepthDriver::instance().getDepthImage();

    if(_depth_image)
    {
      _depth_image->setOrientation(_orientation);
      return 1;
    }
    else
    {
      return 0;
    }
  }
  catchAllAndReturn(0);
}

int depth_image_get_height()
{
  try
  {
    if(_depth_image)
    {
        return _depth_image->getHeight();
    }
    else
    {
        return 0;
    }
  }
  catchAllAndReturn(0);
}

int depth_image_get_width()
{
  try
  {
    if(_depth_image)
    {
        return _depth_image->getWidth();
    }
    else
    {
        return 0;
    }
  }
  catchAllAndReturn(0);
}

int get_depth_value(int x, int y)
{
  try
  {
    if(_depth_image)
    {
      int value = _depth_image->getDepthAt(DepthImageCoordinate(x, y));
      if(value == 0)
      {
        return INVALID_COORDINATE;
      }
      else
      {
        return value;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_COORDINATE);
}

int get_world_x(int x, int y)
{
  try
  {
    if(_depth_image)
    {
      std::unique_ptr<Point> p(_depth_image->getPointAt(DepthImageCoordinate(x, y)));
      if(p)
      {
        return p->getWorldCoordinate().x;
      }
      else
      {
        return INVALID_COORDINATE;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_COORDINATE);
}

int get_world_y(int x, int y)
{
  try
  {
    if(_depth_image)
    {
      std::unique_ptr<Point> p(_depth_image->getPointAt(DepthImageCoordinate(x, y)));
      if(p)
      {
        return p->getWorldCoordinate().y;
      }
      else
      {
        return INVALID_COORDINATE;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_COORDINATE);
}

int get_world_z(int x, int y)
{
  try
  {
    if(_depth_image)
    {
      std::unique_ptr<Point> p(_depth_image->getPointAt(DepthImageCoordinate(x, y)));
      if(p)
      {
        return p->getWorldCoordinate().z;
      }
      else
      {
        return INVALID_COORDINATE;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_COORDINATE);
}