%module kovan
%{
  #include "kovan/kovan.hpp"
  #include "kovan/kovan.h"
  #include "kovan/segment.hpp"
  #include "kovan/colinear_segmenter.hpp"
  #include "kovan/xtion_depth_driver.hpp"
  
  #ifdef SWIGJAVA
  #include "java/motor.hpp"
  #endif
  
  #ifdef SWIGPYTHON
  #define _open open
  #endif
  
  using namespace depth;
%}

#define EXPORT_SYM

#ifdef SWIGPYTHON
#define open _open
#endif

%include "kovan/sensor.hpp"
%template(IntSensor) Sensor<int>;
%template(ShortSensor) Sensor<short>;
%template(UnsignedShortSensor) Sensor<unsigned short>;
%template(BoolSensor) Sensor<bool>;
%include "kovan/motors.hpp"
%include "kovan/geom.hpp"
%include "kovan/port.hpp"
%include "kovan/segment.hpp"
%template(IntSegmenter) Segmenter<int>;
%include "kovan/depth_driver.hpp"
%include "kovan/depth_image.hpp"
%include "kovan/servo.hpp"
%include "kovan/util.hpp"
%include "kovan/xtion_depth_driver.hpp"
%include "kovan/create.hpp"
%include "kovan/color.hpp"
%include "kovan/battery.hpp"
%include "kovan/analog.hpp"
%include "kovan/accel.hpp"
%include "kovan/camera.hpp"
%include "kovan/button.hpp"
%include "kovan/ardrone.hpp"
%include "kovan/config.hpp"
%include "kovan/datalog.hpp"
%include "kovan/colinear_segmenter.hpp"
%include "kovan/digital.hpp"

#ifndef SWIGJAVA
%include "kovan/thread.hpp"

%include "kovan/motors.h"
%include "kovan/geom.h"
%include "kovan/servo.h"
%include "kovan/util.h"
%include "kovan/create.h"
%include "kovan/depth.h"
%include "kovan/battery.h"
%include "kovan/analog.h"
%include "kovan/accel.h"
%include "kovan/camera.h"
%include "kovan/button.h"
%include "kovan/ardrone.h"
%include "kovan/datalog.h"
%include "kovan/console.h"
%include "kovan/audio.h"
%include "kovan/thread.h"
%include "kovan/digital.h"
#endif

// Include various fixups for java
#ifdef SWIGJAVA
// %include "java/camera.i"
%include "java/motor.i"
#endif
