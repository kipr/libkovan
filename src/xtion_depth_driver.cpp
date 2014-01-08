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
#include "kovan/xtion_depth_driver.hpp"
#include "xtion_depth_driver_impl_p.hpp"

using namespace depth;

XtionDepthDriver& XtionDepthDriver::instance()
{
  // TODO: Not thread safe
  static XtionDepthDriver _instance;
  return _instance;
}

XtionDepthDriver::XtionDepthDriver()
  : _impl(new XtionDepthDriverImpl)
{
}

XtionDepthDriver::~XtionDepthDriver()
{
  close();
  delete _impl;
}

void XtionDepthDriver::open()
{
  _impl->open();
}

void XtionDepthDriver::close()
{
  _impl->close();
}


bool XtionDepthDriver::isOpen() const
{
  return _impl->isOpen();
}

DepthResolution XtionDepthDriver::depthCameraResolution() const
{
  return _impl->depthCameraResolution();
}

void XtionDepthDriver::setDepthCameraResolution(const DepthResolution resolution)
{
  _impl->setDepthCameraResolution(resolution);
}

DepthImage *XtionDepthDriver::depthImage() const
{
  return _impl->lastCaptured();
}
