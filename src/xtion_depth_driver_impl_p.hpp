#ifndef _XTION_DEPTH_DRIVER_IMPL_P_HPP_
#define _XTION_DEPTH_DRIVER_IMPL_P_HPP_

#include <OpenNI.h>
VH #include "kovan/depth_resolution.h"
VH #include "kovan/xtion_depth_image.hpp"

namespace depth
{
  class XtionDepthDriverImpl : public openni::OpenNI::DeviceConnectedListener
                             , public openni::OpenNI::DeviceDisconnectedListener
                             , public openni::OpenNI::DeviceStateChangedListener
                             , public openni::VideoStream::NewFrameListener
  {
  public:
    XtionDepthDriverImpl();
    virtual ~XtionDepthDriverImpl();
    
    void open();
    bool isOpen() const;
    void close();
    DepthResolution depthCameraResolution() const;
    void setDepthCameraResolution(const DepthResolution resolution);
    
    XtionDepthImage *lastCaptured();
    
    const openni::VideoStream &stream() const;
    
  private:
    openni::Device _device;
    openni::VideoStream _stream;
    
    XtionDepthImage _lastCaptured;
    
    // Implement OpenNI::DeviceConnectedListener::onDeviceConnected()
    virtual void onDeviceConnected(const openni::DeviceInfo *pInfo);

    // Implement OpenNI::DeviceDisconnectedListener::onDeviceDisconnected()
    virtual void onDeviceDisconnected(const openni::DeviceInfo *pInfo);

    // Implement OpenNI::DeviceStateChangedListener::onDeviceStateChanged()
    virtual void onDeviceStateChanged(const openni::DeviceInfo *pInfo, openni::DeviceState state);

    // Implement VideoStream::NewFrameListener::onNewFrame()
    virtual void onNewFrame(openni::VideoStream &stream);
  };
}

#endif