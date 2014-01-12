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
#include "kovan/colinear_segmenter.hpp"
#include "kovan/depth.h"
#include "kovan/general.h"
#include "kovan/util.h"

#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <exception>
#include <algorithm>
#include <stdlib.h>


namespace depth
{
  namespace Private
  {
    static DepthImage *_depth_image = 0;
    static uint16_t _orientation = 0;
    static int scanRow = -1;
    static std::vector<Segment> segments;
    static SortMethod sortMethod = SORT_NEAREST;
  }
}

using namespace depth;
using namespace depth::Private;

int findMin(const Segment &seg)
{
  if(scanRow < 0) return -1;
  int min = -1;
  int minVal = 0xFFFFFFF;
  for(int i = seg.start; i < seg.end; ++i) {
    if(_depth_image->depthAt(scanRow, i) < minVal) {
      min = i;
      minVal = _depth_image->depthAt(scanRow, i);
    }
  }
  return min;
}

int findMax(const Segment &seg)
{
  if(scanRow < 0) return -1;
  int max = -1;
  int maxVal = 0;
  for(int i = seg.start; i < seg.end; ++i) {
    if(_depth_image->depthAt(scanRow, i) > maxVal) {
      max = i;
      maxVal = _depth_image->depthAt(scanRow, i);
    }
  }
  return max;
}


class SegmentNearestFunctor
{
public:
  SegmentNearestFunctor(int row, depth::DepthImage *const image)
    : _row(row)
    , _image(image)
  {
    
  }
  
  bool operator()(const Segment &a, const Segment &b) const
  {
    return _image->pointAt(_row, findMin(a)).z() < _image->pointAt(_row, findMin(b)).z();
  }
private:

  const int _row;
  depth::DepthImage *const _image;
};

class SegmentFarthestFunctor
{
public:
  SegmentFarthestFunctor(int row, depth::DepthImage *const image)
    : _row(row)
    , _image(image)
  {
    
  }
  
  bool operator()(const Segment &a, const Segment &b) const
  {
    return _image->pointAt(_row, findMax(a)).z() < _image->pointAt(_row, findMax(b)).z();
  }
private:

  const int _row;
  depth::DepthImage *const _image;
};

class SegmentCenterFunctor
{
public:
  SegmentCenterFunctor(int row, depth::DepthImage *const image)
    : _row(row)
    , _image(image)
  {
    
  }
  
  bool operator()(const Segment &a, const Segment &b) const
  {
    return _image->pointAt(_row, (a.start + a.end) >> 1).z()
      < _image->pointAt(_row, (b.start + b.end) >> 1).z();
  }
private:

  const int _row;
  depth::DepthImage *const _image;
};

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
    _depth_image = 0;
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
    segments.clear();
    scanRow = -1;
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

int get_depth_world_point_x(int row, int column)
{
  return get_depth_world_point(row, column).x;
}

int get_depth_world_point_y(int row, int column)
{
  return get_depth_world_point(row, column).y;
}

int get_depth_world_point_z(int row, int column)
{
  return get_depth_world_point(row, column).z;
}



int depth_scanline_update(int row)
{
  if(row < 0 || row >= get_depth_image_height()) {
    std::cerr << "depth_scanline_update needs a valid row" << std::endl;
    return 0;
  }
  scanRow = row;
  int *const data = new int[get_depth_image_width()];
  for(unsigned i = 0; i < get_depth_image_width(); ++i) {
    data[i] = get_depth_value(scanRow, i);
  }
  
  using namespace std;
  ColinearSegmenter segmenter(5);
  vector<Segment> pre = coalesceSegments(segmenter.findSegments(data, get_depth_image_width()));
  segments.clear();
  vector<Segment>::const_iterator it = pre.begin();
  for(; it != pre.end(); ++it) {
    if(data[(*it).start] == 0 || data[(*it).end] == 0) continue;
    if((*it).end - (*it).start < 10) continue;
    segments.push_back(*it);
  }

  if(sortMethod == SORT_NEAREST) {
    std::sort(segments.begin(), segments.end(), SegmentNearestFunctor(scanRow, _depth_image));
  } else if(sortMethod == SORT_CENTER) {
    std::sort(segments.begin(), segments.end(), SegmentCenterFunctor(scanRow, _depth_image));
  } else if(sortMethod == SORT_FARTHEST) {
    std::sort(segments.begin(), segments.end(), SegmentFarthestFunctor(scanRow, _depth_image));
  }
  
  delete[] data;
  return 1;
}

int get_depth_scanline_object_count()
{
  if(!_depth_image) return -1;
  if(scanRow < 0) return -1;
  return segments.size();
}

point3 get_depth_scanline_object_center(int object_num)
{
  if(!_depth_image) return create_point3(-1, -1, -1);
  if(scanRow < 0) {
    std::cerr << "Must call depth_scanline_update first" << std::endl;
    return create_point3(-1, -1, -1);
  }
  if(object_num < 0 || object_num >= segments.size()) {
    std::cerr << "object_num " << object_num << " is invalid!" << std::endl;
    return create_point3(-1, -1, -1);
  }
  return get_depth_world_point(scanRow,
    (segments[object_num].start + segments[object_num].end) >> 1);
}

point3 get_depth_scanline_object_nearest(int object_num)
{
  if(!_depth_image) return create_point3(-1, -1, -1);
  if(scanRow < 0) {
    std::cerr << "Must call depth_scanline_update first" << std::endl;
    return create_point3(-1, -1, -1);
  }
  if(object_num < 0 || object_num >= segments.size()) {
    std::cerr << "object_num " << object_num << " is invalid!" << std::endl;
    return create_point3(-1, -1, -1);
  }
  return get_depth_world_point(scanRow,
    findMin(segments[object_num]));
}

point3 get_depth_scanline_object_farthest(int object_num)
{
  if(!_depth_image) return create_point3(-1, -1, -1);
  if(scanRow < 0) {
    std::cerr << "Must call depth_scanline_update first" << std::endl;
    return create_point3(-1, -1, -1);
  }
  if(object_num < 0 || object_num >= segments.size()) {
    std::cerr << "object_num " << object_num << " is invalid!" << std::endl;
    return create_point3(-1, -1, -1);
  }
  return get_depth_world_point(scanRow,
    findMax(segments[object_num]));
}

int get_depth_scanline_object_center_x(int object_num)
{
  return get_depth_scanline_object_center(object_num).x;
}

int get_depth_scanline_object_center_y(int object_num)
{
  return get_depth_scanline_object_center(object_num).y;
}

int get_depth_scanline_object_center_z(int object_num)
{
  return get_depth_scanline_object_center(object_num).z;
}

int get_depth_scanline_object_nearest_x(int object_num)
{
  return get_depth_scanline_object_nearest(object_num).x;
}

int get_depth_scanline_object_nearest_y(int object_num)
{
  return get_depth_scanline_object_nearest(object_num).y;
}

int get_depth_scanline_object_nearest_z(int object_num)
{
  return get_depth_scanline_object_nearest(object_num).z;
}

int get_depth_scanline_object_farthest_x(int object_num)
{
  return get_depth_scanline_object_farthest(object_num).x;
}

int get_depth_scanline_object_farthest_y(int object_num)
{
  return get_depth_scanline_object_farthest(object_num).y;
}

int get_depth_scanline_object_farthest_z(int object_num)
{
  return get_depth_scanline_object_farthest(object_num).z;
}

int get_depth_scanline_object_size(int object_num)
{
  if(scanRow < 0) {
    std::cerr << "Must call depth_scanline_update first" << std::endl;
    return -1;
  }
  if(object_num < 0 || object_num >= segments.size()) {
    std::cerr << "object_num " << object_num << " is invalid!" << std::endl;
    return -1;
  }
  
  const point3 start = get_depth_world_point(scanRow, segments[object_num].start);
  const point3 end = get_depth_world_point(scanRow, segments[object_num].end);
  
  return sqrt(pow(end.x - start.x, 2) + pow(end.z - start.z, 2));
}

int get_depth_scanline_object_angle(int object_num)
{
  if(scanRow < 0) {
    std::cerr << "Must call depth_scanline_update first" << std::endl;
    return -1;
  }
  
  if(object_num < 0 || object_num >= segments.size()) {
    std::cerr << "object_num " << object_num << " is invalid!" << std::endl;
    return -1;
  }
  
  const point3 start = get_depth_world_point(scanRow, segments[object_num].start);
  const point3 end = get_depth_world_point(scanRow, segments[object_num].end);
  
  return atan2(end.z - start.z, end.x - start.x) * 180.0 / M_PI;
}

void set_depth_scanline_sorting_method(SortMethod method)
{
  sortMethod = method;
}

SortMethod get_depth_scanline_sorting_method()
{
  return sortMethod;
}
