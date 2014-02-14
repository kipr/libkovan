libkovan
========

libkovan is a user library that can read sensor values, move motors, and draw graphics to the Kovan. libkovan is written in C++, but also exposes C bindings for simplified use cases.

Requirements
============

* CMake 2.8.0 or higher
* OpenCV 2.4.8
* OpenNI2
* ZBar
* libav
* i2c_wrapper
* doxygen (optional, for documentation)

Building (OS X and Linux)
=========================

  cd libkovan
  mkdir build
  cd build
  cmake ..
  make
  make install

Authors
=======

* Braden McDorman
* Nafis Zaman

License
=======

libkovan is released under the terms of the GPLv3 license. For more information, see the LICENSE file in the root of this project.
