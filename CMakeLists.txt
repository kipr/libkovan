#################################
#   libkovan CMake Build File   #
#################################

project(libkovan)

cmake_minimum_required(VERSION 2.8.0)

set(INCLUDE ${libkovan_SOURCE_DIR}/include)
set(SRC ${libkovan_SOURCE_DIR}/src)
set(PIXELTOASTER ${SRC}/pixeltoaster)
set(BINDINGS ${CMAKE_SOURCE_DIR}/bindings)
set(TOOLS ${CMAKE_SOURCE_DIR}/tools)

#include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/kovan/*.h ${INCLUDE}/kovan/*.hpp)
file(GLOB SOURCES ${SRC}/*.cpp)
file(GLOB P_INCLUDES ${SRC}/*.h ${SRC}/*.hpp)
file(GLOB PIXELTOASTER_SOURCES ${PIXELTOASTER}/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

#########
# Tools #
#########

if(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../prefix)
endif()

if(KOVAN)
  set(TOOLS_BIN_DIR /usr/local/bin)
else()
  set(TOOLS_BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
endif()

function(run_vtable_gen INPUT NEW_INPUT)
  get_filename_component(FILENAME ${INPUT} NAME)
  set(NEW_INPUT_VAL ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})
  set(${NEW_INPUT} ${NEW_INPUT_VAL} PARENT_SCOPE)
  add_custom_command(OUTPUT ${NEW_INPUT_VAL}
     COMMAND ${TOOLS_BIN_DIR}/vtable_gen ${NEW_INPUT_VAL} ${INPUT}
     DEPENDS ${INPUT})
endfunction()

function(run_vtable_impl INPUT NEW_INPUT)
  get_filename_component(FILENAME ${INPUT} NAME)
  set(NEW_INPUT_VAL ${CMAKE_CURRENT_BINARY_DIR}/vi_${FILENAME})
  set(${NEW_INPUT} ${NEW_INPUT_VAL} PARENT_SCOPE)
  add_custom_command(OUTPUT ${NEW_INPUT_VAL}
     COMMAND ${TOOLS_BIN_DIR}/vtable_impl ${NEW_INPUT_VAL} ${INPUT}
     DEPENDS ${INPUT})
endfunction()

function(run_vtable_impl_h INPUT NEW_INPUT)
  get_filename_component(FILENAME ${INPUT} NAME)
  set(NEW_INPUT_VAL ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})
  set(${NEW_INPUT} ${NEW_INPUT_VAL} PARENT_SCOPE)
  add_custom_command(OUTPUT ${NEW_INPUT_VAL}
     COMMAND ${TOOLS_BIN_DIR}/vtable_impl ${NEW_INPUT_VAL} ${INPUT}
     DEPENDS ${INPUT})
endfunction()

foreach(INCLUDE ${INCLUDES})
  run_vtable_gen(${INCLUDE} VF_INCLUDE)
  list(APPEND VF_INCLUDES ${VF_INCLUDE})
endforeach()

foreach(S ${SOURCES})
  run_vtable_impl(${S} VI_SOURCE)
  list(APPEND VI_SOURCES ${VI_SOURCE})
endforeach()

foreach(S ${P_INCLUDES})
  run_vtable_impl_h(${S} VI_SOURCE)
  list(APPEND VI_P_INCLUDES ${VI_SOURCE})
endforeach()

# REMOVE ME
include_directories(${SRC}/pixeltoaster)

#############################
#   PixelToaster settings   #
#############################

if(APPLE)
	file(GLOB OBJC_SOURCES ${PIXELTOASTER}/*.mm)
elseif(WIN32)
	add_definitions(-fno-check-new)
endif()

############################################
#   Resolve external library directories   #
############################################

if(DEFINED ENV{OPENNI2_INCLUDE_DIR})
  set(OPENNI2_INCLUDE $ENV{OPENNI2_INCLUDE_DIR})
elseif(WIN32)
  set(OPENNI2_INCLUDE "C:/Program Files (x86)/OpenNI2/Include")
else()
  set(OPENNI2_INCLUDE "${CMAKE_SOURCE_DIR}/../OpenNI2/Include")
endif()

if(DEFINED ENV{OPENNI2_LIB_DIR})
  set(OPENNI2_LIB $ENV{OPENNI2_LIB_DIR})
elseif(WIN32)
  set(OPENNI2_LIB "C:/Program Files (x86)/OpenNI2/Lib")
else()
  set(OPENNI2_LIB "${CMAKE_SOURCE_DIR}/../OpenNI2/Bin/x64-Release")
endif()

if(DEFINED ENV{OPENCV_INCLUDE_DIR})
  set(OPENCV_INCLUDE $ENV{OPENCV_INCLUDE_DIR})
elseif(WIN32)
  set(OPENCV_INCLUDE ${CMAKE_SOURCE_DIR}/../opencv/build/install/include)
endif()

if(DEFINED ENV{OPENCV_LIB_DIR})
  set(OPENCV_LIB $ENV{OPENCV_LIB_DIR})
elseif(WIN32)
  set(OPENCV_LIB ${CMAKE_SOURCE_DIR}/../opencv/build/install/x86/vc11/lib)
endif()

if(DEFINED ENV{ZBAR_DIR})
  set(ZBAR_DIR $ENV{ZBAR_DIR})
elseif(WIN32)
  set(ZBAR_DIR "C:/Program Files (x86)/ZBar")
endif()


################################
#   Include/link directories   #
################################

include_directories(${OPENNI2_INCLUDE})
link_directories(${OPENNI2_LIB})
if(WIN32)
  include_directories(${CMAKE_INSTALL_PREFIX}/include)
  include_directories(${OPENCV_INCLUDE})
  include_directories(${ZBAR_DIR}/include)

  link_directories(${CMAKE_INSTALL_PREFIX}/lib)
  link_directories(${OPENCV_LIB})
  link_directories(${ZBAR_DIR}/lib)  
endif()

if(APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
  include_directories(/usr/local/include)
  link_directories(/usr/local/lib)
elseif(WIN32)
  set(SOURCES ${SOURCES} ${CMAKE_SOURCE_DIR}/compat/mingw.cpp)
elseif(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# add_definitions(-O3)

option(build_python "Build python bindings" ON)

file(GLOB SWIG_SOURCES ${BINDINGS}/*.i)

if(build_python)
  find_package(SWIG REQUIRED)
  find_package(PythonLibs REQUIRED)
  message("${PYTHON_LIBRARY}")
  set(PYTHON_BINDING_SOURCES ${CMAKE_BINARY_DIR}/kovan_wrap_py.cxx)
  set(PYTHON_BINDING_LIBRARIES ${PYTHON_LIBRARY})
  include_directories(${PYTHON_INCLUDE_DIR})
  add_custom_command(COMMAND ${SWIG_EXECUTABLE} -Wall -python -c++ -I${INCLUDE} -outdir ${LIBRARY_OUTPUT_PATH} -o ${CMAKE_BINARY_DIR}/kovan_wrap_py.cxx ${BINDINGS}/kovan.i
    DEPENDS ${SWIG_SOURCES}
    OUTPUT ${CMAKE_BINARY_DIR}/kovan_wrap_py.cxx)
endif()

option(build_java "Build java bindings" ON)

if(build_java)
  find_package(SWIG REQUIRED)
  find_package(JNI REQUIRED)
  find_package(Java REQUIRED)
  set(JAVA_BINDING_SOURCES ${CMAKE_BINARY_DIR}/kovan_wrap_java.cxx)
if(APPLE)
  # This is a hack!
  # By default, JNI_LIBRARIES contains libjawt and libjvm. Lib jawt pulls in libawt_lwawt.dylib,
  # which crashes on load into the JVM. To avoid this, we just use libjvm.
  set(JAVA_BINDING_LIBRARIES /Library/Java/JavaVirtualMachines/jdk1.8.0_20.jdk/Contents/Home/jre/lib/server/libjvm.dylib)
else()
  set(JAVA_BINDING_LIBRARIES ${JNI_LIBRARIES})
endif()
  include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${BINDINGS})
  set(JAVA_BASE_PATH ${CMAKE_BINARY_DIR}/java)
  set(JAVA_EXTRAS_PATH ${BINDINGS}/java/org/kipr/kovan)
  set(JAVA_OUT_PATH ${JAVA_BASE_PATH}/org/kipr/kovan)
  set(JAVA_BIN_PATH ${CMAKE_BINARY_DIR}/java_bin)
  file(REMOVE_RECURSE ${JAVA_OUT_PATH})
  file(MAKE_DIRECTORY ${JAVA_OUT_PATH})
  file(REMOVE_RECURSE ${JAVA_BIN_PATH})
  file(MAKE_DIRECTORY ${JAVA_BIN_PATH})
  add_custom_target(swig_java
    COMMAND ${SWIG_EXECUTABLE} -Wall -java -package org.kipr.kovan -c++ -outdir ${JAVA_OUT_PATH} -o ${CMAKE_BINARY_DIR}/kovan_wrap_java.cxx ${BINDINGS}/kovan.i
    DEPENDS ${SWIG_SOURCES})
  add_custom_target(java_compile
    COMMAND ${Java_JAVAC_EXECUTABLE} -d ${JAVA_BIN_PATH} -cp ${JAVA_BIN_PATH} ${JAVA_OUT_PATH}/*.java ${JAVA_EXTRAS_PATH}/*.java
    DEPENDS swig_java)
  add_custom_target(kovan_jar ALL
    COMMAND ${Java_JAR_EXECUTABLE} cf ${LIBRARY_OUTPUT_PATH}/libkovan.jar -C ${JAVA_BIN_PATH}/ .
    DEPENDS swig_java java_compile)
endif()

add_library(kovan SHARED ${VI_SOURCES} ${VI_P_INCLUDES} ${OBJC_SOURCES} ${VF_INCLUDES} ${PIXELTOASTER_SOURCES})


if(build_python)
  add_library(kovan_python SHARED ${PYTHON_BINDING_SOURCES})
  set_target_properties(kovan_python PROPERTIES PREFIX _ OUTPUT_NAME kovan SUFFIX .so)
  target_link_libraries(kovan_python kovan ${PYTHON_BINDING_LIBRARIES})
endif()

if(build_java)
  add_library(kovan_java SHARED ${JAVA_BINDING_SOURCES})
  target_link_libraries(kovan_java kovan ${JAVA_BINDING_LIBRARIES})
endif()

if(WIN32)
  # TODO: Make sure these are current
  target_link_libraries(kovan opencv_core2410 opencv_highgui2410 opencv_imgproc2410 libzbar-0 OpenNI2)
  target_link_libraries(kovan d3d9)
  target_link_libraries(kovan ws2_32)
elseif(KOVAN)
  target_link_libraries(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar avcodec avformat avutil z swscale bz2 OpenNI2)
else()
  target_link_libraries(kovan pthread opencv_core opencv_videoio opencv_imgproc zbar avcodec avformat avutil z swscale bz2 OpenNI2)
endif()

if(APPLE)
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(CORESERVICE_LIBRARY CoreServices)
  target_link_libraries(kovan ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${CORESERVICE_LIBRARY})
endif()

if(KOVAN)
  target_link_libraries(kovan i2c_wrapper)
  add_definitions(-DKOVAN)
endif()

install(FILES ${VF_INCLUDES} DESTINATION include/kovan)
install(TARGETS kovan DESTINATION lib)

########################################
#   Doxygen documentation (optional)   #
########################################

option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" ON)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
  file(MAKE_DIRECTORY doc)
  set(DOXYGEN_DIR ${PROJECT_SOURCE_DIR}/doxygen)
  add_custom_target (Docs++ ALL 
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_DIR}/Doxyfile++ SOURCES ${DOXYGEN_DIR}/Doxyfile++
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_custom_target (Docs ALL 
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_DIR}/Doxyfile SOURCES ${DOXYGEN_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_custom_target(DocMetadata ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${DOXYGEN_DIR}/metadata.ini ${PROJECT_SOURCE_DIR}/doc)
endif()

if(build_tests)
  include_directories(${CMAKE_BINARY_DIR})
  add_executable(test_depth ${CMAKE_SOURCE_DIR}/test.cpp)
  target_link_libraries(test_depth kovan)
endif()
