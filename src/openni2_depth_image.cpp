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

#include "kovan/openni2_depth_image.hpp"
#include "kovan/depth_exception.hpp"

using namespace libkipr_link_depth_sensor;
using namespace openni;

OpenNI2DepthImage::OpenNI2DepthImage(openni::VideoFrameRef video_frame_ref,
                                 const openni::VideoStream& stream)
  : video_frame_ref_(video_frame_ref), stream_(stream),
    size_(0, 0, video_frame_ref_.getWidth(), video_frame_ref_.getHeight()),
    orientation_(0)
{

}

void OpenNI2DepthImage::setOrientation(uint16_t orientation)
{
  orientation_ = orientation;
}

uint16_t OpenNI2DepthImage::getOrientation() const
{
  return orientation_;
}

int32_t OpenNI2DepthImage::getDepthAt(uint32_t row, uint32_t column) const
{
  if(orientation_ == 0)
  {
    return ((DepthPixel*)video_frame_ref_.getData())[(size_.width() - column)
      + row*size_.width()];
  }
  else
  {
    return ((DepthPixel*)video_frame_ref_.getData())[column
      + (size_.height() -row)*size_.width()];
  }
}

uint32_t OpenNI2DepthImage::getWidth() const
{
  return size_.width();
}

uint32_t OpenNI2DepthImage::getHeight() const
{
  return size_.height();
}

Point3<int32_t>* OpenNI2DepthImage::getPointAt(uint32_t row, uint32_t column) const
{
  int depth_value = getDepthAt(row, column);
  float world_x, world_y, world_z;
      
  if(depth_value != 0)
  {
    Status rc = CoordinateConverter::convertDepthToWorld(stream_, 
      (int) column, (int) row, depth_value, &world_x, &world_y, &world_z);
    if(rc != STATUS_OK)
    {
      throw Exception(std::string("Coordinate conversion failed with ")
        + OpenNI::getExtendedError());
    }
    
    return new Point3<int32_t>((int32_t) world_x, (int32_t) world_y, (int32_t) world_z);
  }
  else
  {
    return nullptr;
  }
}
