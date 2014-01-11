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

#include "kovan/xtion_depth_image.hpp"
#include "xtion_depth_driver_impl_p.hpp"
#include "kovan/depth_exception.hpp"

using namespace depth;
using namespace openni;

XtionDepthImage::XtionDepthImage(const void *const data, const uint32_t size, const uint32_t width,
    const uint32_t height, const uint16_t orientation, XtionDepthDriverImpl *const impl)
  : _data(data)
  , _size(size)
  , _width(width)
  , _height(height)
  , _orientation(orientation)
  , _impl(impl)
{
}

XtionDepthImage::~XtionDepthImage()
{
}

void XtionDepthImage::setOrientation(const uint16_t orientation)
{
  // TODO: Check for correct orientation
  _orientation = orientation;
}

uint16_t XtionDepthImage::orientation() const
{
  return _orientation;
}

uint16_t XtionDepthImage::depthAt(const uint32_t row, const uint32_t column) const
{
  if(_orientation == 0) {
    return reinterpret_cast<const DepthPixel *>(_data)[(_width - column) + row * _width];
  }
  
  return reinterpret_cast<const DepthPixel *>(_data)[column + (_height - row) * _width];
}

void XtionDepthImage::depth(uint16_t *const data, const uint32_t offset, const uint32_t size) const
{
  const uint32_t clip = std::max(_width * _height - offset, size);
  memcpy(data, reinterpret_cast<const DepthPixel *>(_data) + offset, size * sizeof(DepthPixel));
}

uint32_t XtionDepthImage::width() const
{
  return _width;
}

uint32_t XtionDepthImage::height() const
{
  return _height;
}

Point3<int32_t> XtionDepthImage::pointAt(const uint32_t row, const uint32_t column) const
{
  const int depth = depthAt(row, column);
  float worldX = 0.0f;
  float worldY = 0.0f;
  float worldZ = 0.0f;
      
  if(depth == 0) return Point3<int32_t>(0, 0, 0);
  
  Status rc = CoordinateConverter::convertDepthToWorld(_impl->stream(), 
    (int)column, (int)row, depth, &worldX, &worldY, &worldZ);
  if(rc != STATUS_OK) {
    throw Exception(std::string("Coordinate conversion failed with ")
      + OpenNI::getExtendedError());
  }
  
  // Center of Xtion offset is 44 mm
  return Point3<int32_t>(worldX - 44, worldY, worldZ);
}

const void *XtionDepthImage::data() const
{
  return _data;
}