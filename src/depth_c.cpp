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
#include "libkipr_link_depth_sensor/ConnectedComponents.hpp"
#include "libkipr_link_depth_sensor/depth.h"

namespace libkipr_link_depth_sensor
{
  namespace c_api
  {
    std::shared_ptr<DepthImage> _depth_image;
    std::unique_ptr<PointCloud> _point_cloud;
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

int add_point_cloud_update_filter(PointCloudUpdateFilter filter, int value)
{
  switch (filter)
  {
  case POINT_CLOUD_FILTER_MIN_X:
    _filter_x.setMin(value);
    return 1;
  case POINT_CLOUD_FILTER_MAX_X:
    _filter_x.setMax(value);
    return 1;
  case POINT_CLOUD_FILTER_MIN_Y:
    _filter_y.setMin(value);
    return 1;
  case POINT_CLOUD_FILTER_MAX_Y:
    _filter_y.setMax(value);
    return 1;
  case POINT_CLOUD_FILTER_MIN_DEPTH:
    _filter_depth.setMin(value);
    return 1;
  case POINT_CLOUD_FILTER_MAX_DEPTH:
    _filter_depth.setMax(value);
    return 1;
    
  default:
    return 0;
  }
}

int reset_point_cloud_update_filter()
{
  _filter_x = MinMaxFilterValue();
  _filter_y = MinMaxFilterValue();
  _filter_depth = MinMaxFilterValue();
  
  return 1;
}

int point_cloud_update()
{
  try
  {
    if(_depth_image)
    {
      _point_cloud.reset(_depth_image->getPointCloud(
        [](const DepthImage* _this, const DepthImageCoordinate& depth_image_coordinate, int& depth) -> bool
        {
          return _filter_x.filter(depth_image_coordinate.x) && _filter_y.filter(depth_image_coordinate.y)
            && _filter_depth.filter(depth);
        }));

      return _point_cloud ? 1 : 0;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(0);
}

int points_in_cloud()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->numberOfPoints();
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(0);
}

int get_cloud_min_x()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->getBoundingBox().min_.x;
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(INVALID_MIN);
}

int get_cloud_min_y()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->getBoundingBox().min_.y;
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(INVALID_MIN);
}

int get_cloud_min_z()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->getBoundingBox().min_.z;
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(INVALID_MIN);
}

int get_cloud_max_x()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->getBoundingBox().max_.x;
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(INVALID_MAX);
}

int get_cloud_max_y()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->getBoundingBox().max_.y;
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(INVALID_MAX);
}

int get_cloud_max_z()
{
  try
  {
    if(_point_cloud)
    {
      return _point_cloud->getBoundingBox().max_.z;
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }
  }
  catchAllAndReturn(INVALID_MAX);
}

int color_cloud(PointCloudColorMode mode)
{
  try
  {
    if(_point_cloud)
    {
      BoundingBox boundingBox = _point_cloud->getBoundingBox();

      switch(mode)
      {
      case POINT_CLOUD_COLOR_MODE_GREY_SCALE:
        _point_cloud->foreach([boundingBox](Point* p)
        {
          uint8_t grey_scale = 0xFF * p->getDepth() / boundingBox.max_.z;
          p->setColor(Color(grey_scale, grey_scale, grey_scale));
        });

        break;
      case POINT_CLOUD_COLOR_MODE_HUE_GRADIENT:
        _point_cloud->foreach([boundingBox](Point* p)
        {
          WorldCoordinate world_coord = p->getWorldCoordinate();

          int32_t h = 6*0xFF*p->getDepth() / boundingBox.max_.z;
          int32_t x = 0xFF - std::abs(h % (2*0xFF) - 0xFF);

          if(h < (1*0xFF))
          {
            p->setColor(Color(0xFF, x, 0x00));
          }
          else if(h < (2*0xFF))
          {
            p->setColor(Color(x, 0xFF, 0x00));
          }
          else if(h < (3*0xFF))
          {
            p->setColor(Color(0x00, 0xFF, x));
          }
          else if(h < (4*0xFF))
          {
            p->setColor(Color(0x00, x, 0xFF));
          }
          else if(h < (5*0xFF))
          {
            p->setColor(Color(x, 0x00, 0xFF));
          }
          else
          {
            p->setColor(Color(0xFF, 0x00, x));
          }
        });

        break;

      case POINT_CLOUD_COLOR_MODE_RGB_GRADIENT:
        _point_cloud->foreach([boundingBox](Point* p)
        {
          WorldCoordinate world_coord = p->getWorldCoordinate();

          uint8_t r = 0xFF * std::abs(world_coord.x - boundingBox.min_.x) / std::abs((boundingBox.max_.x - boundingBox.min_.x));
          uint8_t g = 0xFF * std::abs(world_coord.y - boundingBox.min_.y) / std::abs((boundingBox.max_.y - boundingBox.min_.y));
          uint8_t b = 0xFF * std::abs(world_coord.z - boundingBox.min_.z) / std::abs((boundingBox.max_.z - boundingBox.min_.z));
          p->setColor(Color(r, g, b));
        });

        break;
        
      default:
        throw Exception("Invalid color mode");
      }
    }
    else
    {
        throw Exception("Point cloud is not valid");
    }

    return 1;
  }
  catchAllAndReturn(0);
}

int get_point_color_red(int depth_x, int depth_y)
{
  Point* point = _point_cloud->getPointAtDepthCoordinate(DepthImageCoordinate(depth_x, depth_y));

  if(point)
  {
    return point->getColor().red;
  }
  else
  {
    return 0xFF;
  }
}

int get_point_color_green(int depth_x, int depth_y)
{
  Point* point = _point_cloud->getPointAtDepthCoordinate(DepthImageCoordinate(depth_x, depth_y));

  if(point)
  {
    return point->getColor().green;
  }
  else
  {
    return 0xFF;
  }
}

int get_point_color_blue(int depth_x, int depth_y)
{
  Point* point = _point_cloud->getPointAtDepthCoordinate(DepthImageCoordinate(depth_x, depth_y));

  if(point)
  {
    return point->getColor().blue;
  }
  else
  {
    return 0xFF;
  }
}

std::unique_ptr<ConnectedComponents> _c_c;

int update_conected_component(int delta)
{
  //try
  {
    if(_depth_image)
    {
      _c_c.reset(new ConnectedComponents(*_depth_image, delta));
      
      return 1;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  return 0;
  //catchAllAndReturn(0);
}



int get_conected_component_rgb(int x, int y, int* r, int* g, int* b)
{
  try
  {
    if(_c_c)
    {
      uint32_t id = _c_c->getIdAt(DepthImageCoordinate(x, y));

      *r = (id & 0x0000FF) >>  0;
      *g = (id & 0x0000FF) >>  0;
      *g = (id & 0x0000FF) >>  0;
      
      return 1;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(0);
}
