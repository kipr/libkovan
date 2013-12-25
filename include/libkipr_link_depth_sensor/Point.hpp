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
 * \file Point.hpp
 * \brief This file describes the interface Point
 * \author Stefan Zeltner
 */

#ifndef _POINT_HPP_
#define _POINT_HPP_

#include <stdint.h>

#include "libkipr_link_depth_sensor/WorldCoordinate.hpp"
#include "libkipr_link_depth_sensor/DepthImageCoordinate.hpp"

namespace libkipr_link_depth_sensor
{
  class Point
  {
  public:
    Point(WorldCoordinate world_coord, DepthImageCoordinate depth_coord,
      int32_t depth)
      : world_coord_(world_coord), depth_coord_(depth_coord),
        depth_(depth)
    {

    }

    /**
     * Returns the world coordinate of the point
     *
     * \return The world coordinate of the point
     */
    inline WorldCoordinate getWorldCoordinate() const
    {
      return world_coord_;
    }

    /**
     * Returns the depth image coordinate of the point
     *
     * \return The depth image coordinate of the point
     */
    inline DepthImageCoordinate getDepthImageCoordinate() const
    {
      return depth_coord_;
    }

    /**
     * Returns the depth value of the point
     *
     * \return The depth value of the point
     */
    inline int32_t getDepth() const
    {
      return depth_;
    }

  private:
    DepthImageCoordinate depth_coord_;
    int32_t depth_;

    WorldCoordinate world_coord_;
  };
}

#endif /* _POINT_HPP_ */
