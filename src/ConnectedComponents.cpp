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

#include <list>
#include <vector>
#include <algorithm>

#include "libkipr_link_depth_sensor/ConnectedComponents.hpp"

using namespace libkipr_link_depth_sensor;

ConnectedComponents::ConnectedComponents(const DepthImage& depth_image, uint32_t max_delta)
  : component_map_((depth_image.getWidth() * depth_image.getHeight()), 0),
    size_ (depth_image.getWidth(), depth_image.getHeight())
{
  struct UnionHelper
  {
    uint32_t id;

    UnionHelper* parent;

    UnionHelper(uint32_t id = -1)
      : id(id), parent(nullptr) {}
  };

  uint32_t depth_image_width = depth_image.getWidth();
  uint32_t depth_image_height = depth_image.getHeight();
  
  std::vector<UnionHelper*> component_map_helper((depth_image.getWidth() * depth_image.getHeight()), nullptr);
  static UnionHelper union_helpers[640 * 480];

  int id_cnt = 0;

  for(uint32_t y = 0; y < depth_image_height; y++)
  {
    for(uint32_t x = 0; x < depth_image_width; x++)
    {
      int32_t depth = depth_image.getDepthAt(DepthImageCoordinate(x, y));

      if(depth == 0)
      {
        continue;
      }
      
      // top left corner
      if(x == 0 && y == 0)
      {
        component_map_helper[x + y*depth_image_width] = new (&union_helpers[0]) UnionHelper(0);
      }
      // first row
      else if(y == 0)
      {
        int32_t depth_left = depth_image.getDepthAt(DepthImageCoordinate(x-1, y));;

        if(std::abs(depth_left - depth) <= (int32_t) max_delta)
        {
          component_map_helper[x + y*depth_image_width] = component_map_helper[(x-1) + y*depth_image_width];
        }
        else
        {
          component_map_helper[x + y*depth_image_width] = new (&union_helpers[id_cnt]) UnionHelper(++id_cnt);
        }
      }
      // first column
      else if(x == 0)
      {
        int32_t depth_up = depth_image.getDepthAt(DepthImageCoordinate(x, y-1));;

        if(std::abs(depth_up - depth) <= (int32_t) max_delta)
        {
          component_map_helper[x + y*depth_image_width] = component_map_helper[x + (y-1)*depth_image_width];
        }
        else
        {
          component_map_helper[x + y*depth_image_width] = new (&union_helpers[id_cnt]) UnionHelper(++id_cnt);
        }
      }
      else
      {
        int32_t depth_left = depth_image.getDepthAt(DepthImageCoordinate(x-1, y));;
        int32_t depth_up = depth_image.getDepthAt(DepthImageCoordinate(x, y-1));;

        UnionHelper* id_left = component_map_helper[(x-1) + y*depth_image_width];
        UnionHelper* id_up = component_map_helper[x + (y-1)*depth_image_width];
        
        if(std::abs(depth_left - depth) <= (int32_t) max_delta)
        {
          component_map_helper[x + y*depth_image_width] = id_left;
        }

        if((std::abs(depth_left - depth) <= (int32_t) max_delta) && (std::abs(depth_up - depth) <= (int32_t) max_delta))
        {
          if(id_left != id_up)
          {
            id_left->parent = id_up;
          }
          
          component_map_helper[x + y*depth_image_width] = id_left;
        }
        else if(std::abs(depth_up - depth) <= (int32_t) max_delta)
        {
          component_map_helper[x + y*depth_image_width] = id_up;
        }
        else if(std::abs(depth_left - depth) <= (int32_t) max_delta)
        {
          component_map_helper[x + y*depth_image_width] = id_left;
        }
        else
        {
          component_map_helper[x + y*depth_image_width] = new (&union_helpers[id_cnt]) UnionHelper(++id_cnt);
        }
      }
    }
  }

  for(uint32_t y = 0; y < depth_image_height; y++)
  {
    for(uint32_t x = 0; x < depth_image_width; x++)
    {
      UnionHelper* uh = component_map_helper[x + y*depth_image_width];

      if(uh)
      {
        while(uh->parent)
        {
          uh = uh->parent;
        }

        component_map_[x + y*depth_image_width] = uh->id;
      }
      else
      {
        component_map_[x + y*depth_image_width] = 0xFFFFFFFF;
      }
    }
  }
}

uint32_t ConnectedComponents::getIdAt(DepthImageCoordinate coordinate)
{
  return component_map_[coordinate.x + size_.width*coordinate.y];
}
