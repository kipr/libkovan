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
 * \file DepthImageSize.hpp
 * \brief This file describes the struct DepthImageSize
 * \author Stefan Zeltner
 */

#ifndef _DEPTH_IMAGE_SIZE_HPP_
#define _DEPTH_IMAGE_SIZE_HPP_

#include <stdint.h>

namespace libkipr_link_depth_sensor
{
  struct DepthImageSize
  {
    uint32_t width;
    uint32_t height;

    DepthImageSize(uint32_t width, uint32_t height)
      : width(width), height(height) {}
  };
}

#endif /* _DEPTH_IMAGE_SIZE_HPP_ */
