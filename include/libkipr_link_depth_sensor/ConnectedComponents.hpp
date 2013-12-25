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
 * \file ConnectedComponents.hpp
 * \brief The class ConnectedComponents
 * \author Stefan Zeltner
 */

#ifndef _CONNECTED_COMPONENTS_HPP_
#define _CONNECTED_COMPONENTS_HPP_

#include <vector>

#include <libkipr_link_depth_sensor/Point.hpp>
#include <libkipr_link_depth_sensor/DepthImage.hpp>
#include <libkipr_link_depth_sensor/DepthImageSize.hpp>

namespace libkipr_link_depth_sensor
{
  class ConnectedComponents
  {
  public:
    ConnectedComponents(const DepthImage& depth_image, uint32_t max_delta);

    uint32_t getIdAt(DepthImageCoordinate coordinate);

  private:
    std::vector<uint32_t> component_map_;
    DepthImageSize size_;
  };
}

#endif /* _CONNECTED_COMPONENTS_HPP_ */
