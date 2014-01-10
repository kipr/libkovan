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

#include "kovan/depth_exception.hpp"
#include "kovan/depth_driver.hpp"
#include "kovan/depth.h"
#include "kovan/general.h"
#include "kovan/util.h"

#include <iostream>
#include <exception>

namespace depth
{
  namespace Private
  {
    static DepthImage *_depth_image = 0;
    static uint16_t _orientation = 0;
  }
}

using namespace depth;
using namespace depth::Private;

#define catchAllAndReturn(return_value) \
  catch(std::exception& e) { std::cerr << e.what() << std::endl; } \
  catch(const char* msg) { std::cerr << msg << std::endl; } \
  catch(...) {} \
  return (return_value)

int depth_open()
{
  try {
    DepthDriver::instance().open();
    const double begin = seconds();
    while(!depth_update() && seconds() < begin + 2.0) msleep(5);
    return depth_update();
  }
  catchAllAndReturn(0);
}

int depth_close()
{
  try {
    DepthDriver::instance().close();
    return 1;
  }
  catchAllAndReturn(0);
}

DepthResolution get_depth_resolution()
{
  try {
    return DepthDriver::instance().depthCameraResolution();
  }
  catchAllAndReturn(DEPTH_INVALID_RESOLUTION);
}

int set_depth_resolution(DepthResolution resolution)
{
  try {
    DepthDriver::instance().setDepthCameraResolution(resolution);
    return 1;
  }
  catchAllAndReturn(0);
}

int set_depth_orientation(uint16_t orientation)
{
  try {
    _orientation = orientation;
    return 1;
  }
  catchAllAndReturn(0);
}

int get_depth_orientation()
{
  try {
    return _orientation;
  }
  catchAllAndReturn(0xFFFF);
}

int depth_update()
{
  try {
    _depth_image = DepthDriver::instance().depthImage();
    if(!_depth_image) return 0;
    _depth_image->setOrientation(_orientation);
    return 1;
  }
  catchAllAndReturn(0);
}

int get_depth_image_height()
{
  try {
    return _depth_image ? _depth_image->height() : 0;
  }
  catchAllAndReturn(0);
}

int get_depth_image_width()
{
  try {
    return _depth_image ? _depth_image->width() : 0;
  }
  catchAllAndReturn(0);
}

int get_depth_value(int row, int column)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    return _depth_image->depthAt(row, column);
  }
  catchAllAndReturn(-1);
}

point3 get_depth_world_point(int row, int column)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    Point3<int32_t> p(_depth_image->pointAt(row, column));
    return p.toCPoint3();
  }
  catchAllAndReturn(create_point3(-1, -1, -1));
}

int depth_scan_line_min(int row)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    const uint32_t width = _depth_image->width();
    uint32_t min = INVALID_DEPTH;

    for(uint32_t c = 0; c < width; ++c) {
      const uint32_t depth = _depth_image->depthAt(row, c);
      if(!depth || depth >= min) continue;
      
      min = depth;
    }

    return min;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_min_mask(int row, int min_distance, int max_distance)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    const uint32_t width = _depth_image->width();
    uint32_t min = INVALID_DEPTH;

    for(uint32_t c = 0; c < width; ++c) {
      const uint32_t depth = _depth_image->depthAt(row, c);

      if(depth && (depth < min) && (depth >= min_distance) && (depth <= max_distance)) {
        min = depth;
      }
    }

    return min;

  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_max(int row)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t width = _depth_image->width();
    uint32_t max = 0;

    for(uint32_t c = 0; c < width; ++c) {
      const uint32_t depth = _depth_image->depthAt(row, c);
      if(!depth || depth <= max) continue;
      
      max = depth;
    }

    return max ? max : INVALID_DEPTH;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_max_mask(int row, int min_distance, int max_distance)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    const uint32_t width = _depth_image->width();
    uint32_t max = 0;

    for(uint32_t c = 0; c < width; ++c) {
      const uint32_t depth = _depth_image->depthAt(row, c);

      if(depth && (depth > max) && (depth >= min_distance) && (depth <= max_distance)) {
        max = depth;
      }
    }

    return max ? max : INVALID_DEPTH;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_mean(int row)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    const uint32_t width = _depth_image->width();
    uint32_t mean_sum = 0;
    uint32_t mean_cnt = 0;

    for(uint32_t c = 0; c < width; ++c) {
      const uint32_t depth = _depth_image->depthAt(row, c);
      if(!depth) continue;
      
      mean_sum += depth;
      mean_cnt++;
    }

    return mean_sum / mean_cnt;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_scan_line_mean_mask(int row, int min_distance, int max_distance)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    const uint32_t width = _depth_image->width();
    uint32_t mean_sum = 0;
    uint32_t mean_cnt = 0;

    for(uint32_t c = 0; c < width; ++c) {
      const uint32_t depth = _depth_image->depthAt(row, c);

      if(depth && (depth >= min_distance) && (depth <= max_distance)) {
        mean_sum += depth;
        mean_cnt++;
      }
    }

    return mean_cnt ? (mean_sum / mean_cnt) : INVALID_DEPTH;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_min(int row_from, int row_to,
                           int column_from, int column_to)
{
  try
  {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t min = INVALID_DEPTH;
    
    if(row_from < 0) row_from = 0;
    if(row_to > _depth_image->width()) row_to = _depth_image->width();
    
    if(column_from < 0) column_from = 0;
    if(column_to > _depth_image->height()) column_to = _depth_image->height();

    for(uint32_t r = row_from; r < row_to; ++r) {
      for(uint32_t c = column_from; c < column_to; ++c) {
        uint32_t depth = _depth_image->depthAt(r, c);
        if(!depth || depth >= min) continue;
        
        min = depth;
      }

      return min;
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_min_mask(int row_from, int row_to,
                                int column_from, int column_to,
                                int min_distance, int max_distance)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t min = INVALID_DEPTH;
    
    if(row_from < 0) row_from = 0;
    if(row_to > _depth_image->width()) row_to = _depth_image->width();
    
    if(column_from < 0) column_from = 0;
    if(column_to > _depth_image->height()) column_to = _depth_image->height();
    

    for(uint32_t r = row_from; r < row_to; ++r) {
      for(uint32_t c = column_from; c < column_to; ++c) {
        const uint32_t depth = _depth_image->depthAt(r, c);

        if(depth && (depth < min) && (depth >= min_distance) && (depth <= max_distance)) {
          min = depth;
        }
      }

      return min;
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_max(int row_from, int row_to,
                           int column_from, int column_to)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t max = 0;
    
    if(row_from < 0) row_from = 0;
    if(row_to > _depth_image->width()) row_to = _depth_image->width();
    
    if(column_from < 0) column_from = 0;
    if(column_to > _depth_image->height()) column_to = _depth_image->height();
    
    for(uint32_t r = row_from; r < row_to; ++r) {
      for(uint32_t c = column_from; c < column_to; ++c) {
        const uint32_t depth = _depth_image->depthAt(r, c);
        if(!depth || depth <= max) continue;
        
        max = depth;
      }
    }
    
    return max ? max : INVALID_DEPTH;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_max_mask(int row_from, int row_to,
                                int column_from, int column_to,
                                int min_distance, int max_distance)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t max = 0;
    
    if(row_from < 0) row_from = 0;
    if(row_to > _depth_image->width()) row_to = _depth_image->width();
    
    if(column_from < 0) column_from = 0;
    if(column_to > _depth_image->height()) column_to = _depth_image->height();

    for(uint32_t r = row_from; r < row_to; ++r) {
      for(uint32_t c = column_from; c < column_to; ++c) {
        const uint32_t depth = _depth_image->depthAt(r, c);

        if(depth && (depth > max) && (depth >= min_distance) && (depth <= max_distance)) {
          max = depth;
        }
      }

      return max ? max : INVALID_DEPTH;
    }
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_mean(int row_from, int row_to,
                            int column_from, int column_to)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t mean_sum = 0;
    uint32_t mean_cnt = 0;
    
    if(row_from < 0) row_from = 0;
    if(row_to > _depth_image->width()) row_to = _depth_image->width();
    
    if(column_from < 0) column_from = 0;
    if(column_to > _depth_image->height()) column_to = _depth_image->height();
    
    for(uint32_t r = row_from; r < row_to; ++r) {
      for(uint32_t c = column_from; c < column_to; ++c) {
        const uint32_t depth = _depth_image->depthAt(r, c);
        if(!depth) continue;
        
        mean_sum += depth;
        mean_cnt++;
      }
    }
    return mean_cnt ? (mean_sum / mean_cnt) : INVALID_DEPTH;
  }
  catchAllAndReturn(INVALID_DEPTH);
}

int depth_bounding_box_mean_mask(int row_from, int row_to,
                                 int column_from, int column_to,
                                 int min_distance, int max_distance)
{
  try {
    if(!_depth_image) throw Exception("Depth image is not valid");
    
    uint32_t mean_sum = 0;
    uint32_t mean_cnt = 0;
    
    if(row_from < 0) row_from = 0;
    if(row_to > _depth_image->width()) row_to = _depth_image->width();
    
    if(column_from < 0) column_from = 0;
    if(column_to > _depth_image->height()) column_to = _depth_image->height();
    

    for(uint32_t r = row_from; r < row_to; ++r) {
      for(uint32_t c = column_from; c < column_to; ++c) {
        const uint32_t depth = _depth_image->depthAt(r, c);

        if(depth && (depth >= min_distance) && (depth <= max_distance)) {
          mean_sum += depth;
          mean_cnt++;
        }
      }
    }
    
    return mean_cnt ? (mean_sum / mean_cnt) : INVALID_DEPTH;
  }
  catchAllAndReturn(INVALID_DEPTH);
}
