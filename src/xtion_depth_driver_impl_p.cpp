#include "xtion_depth_driver_impl_p.hpp"
#include <kovan/depth_exception.hpp>

using namespace depth;
using namespace openni;

XtionDepthDriverImpl::XtionDepthDriverImpl()
  : _lastCaptured(0, 0, 0, 0, 0, 0)
{
  const Status rc = OpenNI::initialize();
  if(rc != STATUS_OK) {
    throw Exception(std::string("Initialize failed with ")
      + OpenNI::getExtendedError());
  }
  OpenNI::addDeviceConnectedListener(this);
  OpenNI::addDeviceDisconnectedListener(this);
  OpenNI::addDeviceStateChangedListener(this);
}

XtionDepthDriverImpl::~XtionDepthDriverImpl()
{
  OpenNI::removeDeviceConnectedListener(this);
  OpenNI::removeDeviceDisconnectedListener(this);
  OpenNI::removeDeviceStateChangedListener(this);
  OpenNI::shutdown();
}

void XtionDepthDriverImpl::open()
{
  if(_device.isValid()) return;

  Status rc = _device.open(ANY_DEVICE);
  if(rc != STATUS_OK) {
    throw Exception(std::string("Open the device failed with\n")
      + OpenNI::getExtendedError());
  }
  _device.open(ANY_DEVICE);

  if(!_device.getSensorInfo(SENSOR_DEPTH)) {
    _device.close();
    throw Exception("Device has no depth sensor!");
  }

  rc = _stream.create(_device, SENSOR_DEPTH);
  if(rc != STATUS_OK) {
    _device.close();
    throw Exception(std::string("Create the depth stream failed with\n")
      + OpenNI::getExtendedError());
  }

  VideoMode mode = _stream.getVideoMode();
  mode.setPixelFormat(PIXEL_FORMAT_DEPTH_1_MM);

  rc = _stream.setVideoMode(mode);
  if(rc != STATUS_OK) {
      throw Exception(std::string("Set the pixel format to "
          "PIXEL_FORMAT_DEPTH_1_MM failed with\n")
          + OpenNI::getExtendedError());
  }

  rc = _stream.start();
  if(rc != STATUS_OK) {
    _stream.destroy();
    _device.close();

    throw Exception(std::string("Starting the depth stream failed with\n")
      + OpenNI::getExtendedError());
  }

  rc = _stream.addNewFrameListener(this);
  if(rc != STATUS_OK) {
    _stream.stop();
    _stream.destroy();
    _device.close();

    throw Exception(std::string("Adding the frame listener failed with\n")
      + OpenNI::getExtendedError());
  }
}

bool XtionDepthDriverImpl::isOpen() const
{
  return _device.isValid();
}

void XtionDepthDriverImpl::close()
{
  if(!isOpen()) return;
  
  _stream.removeNewFrameListener(this);

  _stream.stop();
  _stream.destroy();

  _device.close();
}

DepthResolution XtionDepthDriverImpl::depthCameraResolution() const
{
  const VideoMode mode = _stream.getVideoMode();

  int res_x = mode.getResolutionX();
  int res_y = mode.getResolutionY();

  if((res_x == 320) && (res_y = 240)) {
    return DEPTH_RESOLUTION_320_240;
  } else if((res_x == 640) && (res_y = 480)) {
    return DEPTH_RESOLUTION_640_480;
  }
  
  return DEPTH_INVALID_RESOLUTION;
}

void XtionDepthDriverImpl::setDepthCameraResolution(const DepthResolution resolution)
{
  _stream.stop();

  VideoMode mode = _stream.getVideoMode();

  if(resolution == DEPTH_RESOLUTION_320_240) {
    mode.setResolution(320, 240);
  } else if(resolution == DEPTH_RESOLUTION_640_480) {
    mode.setResolution(640, 480);
  } else {
    _stream.start();
    throw Exception("Invalid resolution");
  }

  Status rc = _stream.setVideoMode(mode);
  if (rc != STATUS_OK) {
    _stream.start();
    throw Exception(std::string("Set the resolution failed with\n")
      + OpenNI::getExtendedError());
  }
  
  rc = _stream.start();
  if(rc != STATUS_OK) {
    // how handle this?!
    close();
    
    throw Exception("Unable to start the depth stream");
  }
}

XtionDepthImage *XtionDepthDriverImpl::lastCaptured()
{
  return _lastCaptured.data() ? &_lastCaptured : 0;
}

const openni::VideoStream &XtionDepthDriverImpl::stream() const
{
  return _stream;
}

void XtionDepthDriverImpl::onDeviceConnected(const DeviceInfo *pInfo)
{
}

void XtionDepthDriverImpl::onDeviceDisconnected(const DeviceInfo *pInfo)
{
}

void XtionDepthDriverImpl::onDeviceStateChanged(const DeviceInfo *pInfo, DeviceState state)
{
}

void XtionDepthDriverImpl::onNewFrame(VideoStream &stream)
{
  VideoFrameRef ref;
  stream.readFrame(&ref);
  _lastCaptured = XtionDepthImage(ref.getData(), ref.getDataSize(),
    ref.getWidth(), ref.getHeight(), 0, this);
}