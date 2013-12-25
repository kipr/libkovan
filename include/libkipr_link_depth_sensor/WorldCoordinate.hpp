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
 * \file WorldCoordinate.hpp
 * \brief This file describes the struct WorldCoordinate
 * \author Stefan Zeltner
 */

#ifndef _WORLD_COORDINATE_HPP_
#define _WORLD_COORDINATE_HPP_

#include <stdint.h>

namespace libkipr_link_depth_sensor
{
  struct WorldCoordinate
  {
    int32_t x;
    int32_t y;
    int32_t z;

    WorldCoordinate(int32_t x, int32_t y, int32_t z)
      : x(x), y(y), z(z) {}

    inline bool operator>=(const WorldCoordinate& rhs) const
    {
      return (x >= rhs.x)
        && (y >= rhs.y)
        && (z >= rhs.z);
    }

    inline bool operator<=(const WorldCoordinate& rhs) const
    {
      return (x <= rhs.x)
        && (y <= rhs.y)
        && (z <= rhs.z);
    }
  };
}

#endif /* _WORLD_COORDINATE_HPP_ */
