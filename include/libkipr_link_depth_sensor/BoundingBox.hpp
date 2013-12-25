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
 * \file BoundingBox.hpp
 * \brief This file describes the struct BoundingBox
 * \author Stefan Zeltner
 */

#ifndef _BOUNDING_BOX_HPP_
#define _BOUNDING_BOX_HPP_

#include <libkipr_link_depth_sensor/Point.hpp>

namespace libkipr_link_depth_sensor
{
  struct BoundingBox
  {
    WorldCoordinate min_;
    WorldCoordinate max_;

    BoundingBox()
      : min_(WorldCoordinate(INT32_MAX,INT32_MAX,INT32_MAX)),
        max_(WorldCoordinate(INT32_MIN, INT32_MIN, INT32_MIN)) {}

    inline bool contains(const Point& p)
    {
      return p.getWorldCoordinate() >= min_ && p.getWorldCoordinate() <= max_;
    }

    inline void add(const Point& p)
    {
      WorldCoordinate p_coord = p.getWorldCoordinate();

      if(min_.x > p_coord.x)
      {
        min_.x = p_coord.x;
      }
      if(min_.y > p_coord.y)
      {
        min_.y = p_coord.y;
      }
      if(min_.z > p_coord.z)
      {
        min_.z = p_coord.z;
      }
      
      if(max_.x < p_coord.x)
      {
        max_.x = p_coord.x;
      }
      if(max_.y < p_coord.y)
      {
        max_.y = p_coord.y;
      }
      if(max_.z < p_coord.z)
      {
        max_.z = p_coord.z;
      }
    }
  };
}

#endif /* _BOUNDING_BOX_HPP_ */
