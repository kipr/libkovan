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

#include <iostream>
#include <exception>
#include <memory>

#include "kovan/depth_exception.hpp"
#include "kovan/depth_driver.hpp"
#include "kovan/depth.h"

namespace depth
{
  namespace c_api
  {
    std::shared_ptr<DepthImage> _depth_image;
    uint16_t _orientation = 0;
  }
}

using namespace depth;
using namespace depth::c_api;

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

DepthResolution get_depth_camera_resolution()
{
  try
  {
    return DepthDriver::instance().getDepthCameraResolution();
  }
  catchAllAndReturn(DEPTH_INVALID_RESOLUTION);
}

int set_depth_camera_resolution(DepthResolution resolution)
{
  try
  {
    DepthDriver::instance().setDepthCameraResolution(resolution);

    return 1;
  }
  catchAllAndReturn(0);
}

int set_depth_camera_orientation(uint16_t orientation)
{
  try
  {
    _orientation = orientation;

    return 1;
  }
  catchAllAndReturn(0);
}

int get_depth_camera_orientation()
{
  try
  {
    return _orientation;
  }
  catchAllAndReturn(0xFFFF);
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

int get_depth_value(int row, int column)
{
  try
  {
    if(_depth_image)
    {
      int value = _depth_image->getDepthAt(row, column);
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

int get_world_x(int row, int column)
{
  try
  {
    if(_depth_image)
    {
      std::unique_ptr<Point3<int32_t>> p(_depth_image->getPointAt(row, column));
      if(p)
      {
        return p->x();
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

int get_world_y(int row, int column)
{
  try
  {
    if(_depth_image)
    {
      std::unique_ptr<Point3<int32_t>> p(_depth_image->getPointAt(row, column));
      if(p)
      {
        return p->y();
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

int get_world_z(int row, int column)
{
  try
  {
    if(_depth_image)
    {
      std::unique_ptr<Point3<int32_t>> p(_depth_image->getPointAt(row, column));
      if(p)
      {
        return p->z();
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

int depth_scan_line_min(int row)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c;
      uint32_t width = _depth_image->getWidth();
      uint32_t min = INVALID_DEPTH;
      uint32_t depth;

      for(c = 0; c < width; c++)
      {
        depth = _depth_image->getDepthAt(row, c);

        if(depth && (depth < min))
        {
          min = depth;
        }
      }

      return min;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_min_mask(int row, int min_distance, int max_distance)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c;
      uint32_t width = _depth_image->getWidth();
      uint32_t min = INVALID_DEPTH;
      uint32_t depth;

      for(c = 0; c < width; c++)
      {
        depth = _depth_image->getDepthAt(row, c);

        if(depth && (depth < min) && (depth >= min_distance) && (depth <= max_distance))
        {
          min = depth;
        }
      }

      return min;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_max(int row)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c;
      uint32_t width = _depth_image->getWidth();
      uint32_t max = 0;
      uint32_t depth;

      for(c = 0; c < width; c++)
      {
        depth = _depth_image->getDepthAt(row, c);

        if(depth && (depth > max))
        {
          max = depth;
        }
      }

      return max ? max : INVALID_DEPTH;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_max_mask(int row, int min_distance, int max_distance)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c;
      uint32_t width = _depth_image->getWidth();
      uint32_t max = 0;
      uint32_t depth;

      for(c = 0; c < width; c++)
      {
        depth = _depth_image->getDepthAt(row, c);

        if(depth && (depth > max) && (depth >= min_distance) && (depth <= max_distance))
        {
          max = depth;
        }
      }

      return max ? max : INVALID_DEPTH;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_mean(int row)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c;
      uint32_t width = _depth_image->getWidth();
      uint32_t mean_sum = 0;
      uint32_t mean_cnt = 0;
      uint32_t depth;

      for(c = 0; c < width; c++)
      {
        depth = _depth_image->getDepthAt(row, c);

        if(depth)
        {
          mean_sum += depth;
          mean_cnt++;
        }
      }

      return mean_sum / mean_cnt;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_mean_mask(int row, int min_distance, int max_distance)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c;
      uint32_t width = _depth_image->getWidth();
      uint32_t mean_sum = 0;
      uint32_t mean_cnt = 0;
      uint32_t depth;

      for(c = 0; c < width; c++)
      {
        depth = _depth_image->getDepthAt(row, c);

        if(depth && (depth >= min_distance) && (depth <= max_distance))
        {
          mean_sum += depth;
          mean_cnt++;
        }
      }

      return mean_cnt ? (mean_sum / mean_cnt) : INVALID_DEPTH;
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_min(int row_from, int row_to,
                           int column_from, int column_to)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c, r;
      uint32_t min = INVALID_DEPTH;
      uint32_t depth;
      
      if(row_from < 0)
      {
        row_from = 0;
      }
      if(row_to > _depth_image->getWidth())
      {
        row_to = _depth_image->getWidth();
      }
      if(column_from < 0)
      {
        column_from = 0;
      }
      if(column_to > _depth_image->getHeight())
      {
        column_to = _depth_image->getHeight();
      }

      for(r = row_from; r < row_to; r++)
      {
        for(c = column_from; c < column_to; c++)
        {
          depth = _depth_image->getDepthAt(r, c);

          if(depth && (depth < min))
          {
            min = depth;
          }
        }

        return min;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_min_mask(int row_from, int row_to,
                                int column_from, int column_to,
                                int min_distance, int max_distance)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c, r;
      uint32_t min = INVALID_DEPTH;
      uint32_t depth;
      
      if(row_from < 0)
      {
        row_from = 0;
      }
      if(row_to > _depth_image->getWidth())
      {
        row_to = _depth_image->getWidth();
      }
      if(column_from < 0)
      {
        column_from = 0;
      }
      if(column_to > _depth_image->getHeight())
      {
        column_to = _depth_image->getHeight();
      }

      for(r = row_from; r < row_to; r++)
      {
        for(c = column_from; c < column_to; c++)
        {
          depth = _depth_image->getDepthAt(r, c);

          if(depth && (depth < min) && (depth >= min_distance) && (depth <= max_distance))
          {
            min = depth;
          }
        }

        return min;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_max(int row_from, int row_to,
                           int column_from, int column_to)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c, r;
      uint32_t max = 0;
      uint32_t depth;
      
      if(row_from < 0)
      {
        row_from = 0;
      }
      if(row_to > _depth_image->getWidth())
      {
        row_to = _depth_image->getWidth();
      }
      if(column_from < 0)
      {
        column_from = 0;
      }
      if(column_to > _depth_image->getHeight())
      {
        column_to = _depth_image->getHeight();
      }

      for(r = row_from; r < row_to; r++)
      {
        for(c = column_from; c < column_to; c++)
        {
          depth = _depth_image->getDepthAt(r, c);

          if(depth && (depth > max))
          {
            max = depth;
          }
        }

        return max ? max : INVALID_DEPTH;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_max_mask(int row_from, int row_to,
                                int column_from, int column_to,
                                int min_distance, int max_distance)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c, r;
      uint32_t max = 0;
      uint32_t depth;
      
      if(row_from < 0)
      {
        row_from = 0;
      }
      if(row_to > _depth_image->getWidth())
      {
        row_to = _depth_image->getWidth();
      }
      if(column_from < 0)
      {
        column_from = 0;
      }
      if(column_to > _depth_image->getHeight())
      {
        column_to = _depth_image->getHeight();
      }

      for(r = row_from; r < row_to; r++)
      {
        for(c = column_from; c < column_to; c++)
        {
          depth = _depth_image->getDepthAt(r, c);

          if(depth && (depth > max) && (depth >= min_distance) && (depth <= max_distance))
          {
            max = depth;
          }
        }

        return max ? max : INVALID_DEPTH;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_mean(int row_from, int row_to,
                            int column_from, int column_to)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c, r;
      uint32_t mean_sum = 0;
      uint32_t mean_cnt = 0;
      uint32_t depth;
      
      if(row_from < 0)
      {
        row_from = 0;
      }
      if(row_to > _depth_image->getWidth())
      {
        row_to = _depth_image->getWidth();
      }
      if(column_from < 0)
      {
        column_from = 0;
      }
      if(column_to > _depth_image->getHeight())
      {
        column_to = _depth_image->getHeight();
      }

      for(r = row_from; r < row_to; r++)
      {
        for(c = column_from; c < column_to; c++)
        {
          depth = _depth_image->getDepthAt(r, c);

          if(depth)
          {
            mean_sum += depth;
            mean_cnt++;
          }
        }

        return mean_cnt ? (mean_sum / mean_cnt) : INVALID_DEPTH;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_mean_mask(int row_from, int row_to,
                                 int column_from, int column_to,
                                 int min_distance, int max_distance)
{
  try
  {
    if(_depth_image)
    {
      uint32_t c, r;
      uint32_t mean_sum = 0;
      uint32_t mean_cnt = 0;
      uint32_t depth;
      
      if(row_from < 0)
      {
        row_from = 0;
      }
      if(row_to > _depth_image->getWidth())
      {
        row_to = _depth_image->getWidth();
      }
      if(column_from < 0)
      {
        column_from = 0;
      }
      if(column_to > _depth_image->getHeight())
      {
        column_to = _depth_image->getHeight();
      }

      for(r = row_from; r < row_to; r++)
      {
        for(c = column_from; c < column_to; c++)
        {
          depth = _depth_image->getDepthAt(r, c);

          if(depth && (depth >= min_distance) && (depth <= max_distance))
          {
            mean_sum += depth;
            mean_cnt++;
          }
        }

        return mean_cnt ? (mean_sum / mean_cnt) : INVALID_DEPTH;
      }
    }
    else
    {
        throw Exception("Depth image is not valid");
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}
