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

add_executable(vtable_gen ${TOOLS}/vtable_gen.cpp)
add_executable(vtable_impl ${TOOLS}/vtable_impl.cpp)

if(WIN32)
  set(TOOLS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR}/Release)
else()
  set(TOOLS_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
endif()

function(run_vtable_gen INPUT NEW_INPUT)
  get_filename_component(FILENAME ${INPUT} NAME)
  set(NEW_INPUT_VAL ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})
  set(${NEW_INPUT} ${NEW_INPUT_VAL} PARENT_SCOPE)
  add_custom_command(OUTPUT ${NEW_INPUT_VAL}
     COMMAND ${TOOLS_BIN_DIR}/vtable_gen ${NEW_INPUT_VAL} ${INPUT}
     DEPENDS vtable_gen ${INPUT})
endfunction()

function(run_vtable_impl INPUT NEW_INPUT)
  get_filename_component(FILENAME ${INPUT} NAME)
  set(NEW_INPUT_VAL ${CMAKE_CURRENT_BINARY_DIR}/vi_${FILENAME})
  set(${NEW_INPUT} ${NEW_INPUT_VAL} PARENT_SCOPE)
  add_custom_command(OUTPUT ${NEW_INPUT_VAL}
     COMMAND ${TOOLS_BIN_DIR}/vtable_impl ${NEW_INPUT_VAL} ${INPUT}
     DEPENDS vtable_impl ${INPUT})
endfunction()

function(run_vtable_impl_h INPUT NEW_INPUT)
  get_filename_component(FILENAME ${INPUT} NAME)
  set(NEW_INPUT_VAL ${CMAKE_CURRENT_BINARY_DIR}/${FILENAME})
  set(${NEW_INPUT} ${NEW_INPUT_VAL} PARENT_SCOPE)
  add_custom_command(OUTPUT ${NEW_INPUT_VAL}
     COMMAND ${TOOLS_BIN_DIR}/vtable_impl ${NEW_INPUT_VAL} ${INPUT}
     DEPENDS vtable_impl ${INPUT})
endfunction()

foreach(INCLUDE ${INCLUDES})
  run_vtable_gen(${INCLUDE} VF_INCLUDE)
  list(APPEND VF_INCLUDES ${VF_INCLUDE})
endforeach()
message(${VF_INCLUDES})

foreach(S ${SOURCES})
  run_vtable_impl(${S} VI_SOURCE)
  list(APPEND VI_SOURCES ${VI_SOURCE})
endforeach()

foreach(S ${P_INCLUDES})
  MESSAGE(${S})
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

if(WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/../prefix)
message("SETTING INSTALL PREFIX")
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
  list(APPEND BINDINGS_SOURCES ${CMAKE_BINARY_DIR}/kovan_wrap_py.cxx)
  list(APPEND BINDINGS_LIBRARIES ${PYTHON_LIBRARY})
  include_directories(${PYTHON_INCLUDE_DIR})
  add_custom_command(COMMAND ${SWIG_EXECUTABLE} -Wall -python -c++ -I${INCLUDE} -outdir ${LIBRARY_OUTPUT_PATH} -o ${CMAKE_BINARY_DIR}/kovan_wrap_py.cxx ${BINDINGS}/kovan.i
    DEPENDS ${SWIG_SOURCES}
    OUTPUT ${CMAKE_BINARY_DIR}/kovan_wrap_py.cxx)
endif()

option(build_java "Build java bindings" ON)

if(build_java)
  find_package(SWIG REQUIRED)
  find_package(JNI REQUIRED)
  list(APPEND BINDINGS_SOURCES ${CMAKE_BINARY_DIR}/kovan_wrap_java.cxx)
  list(APPEND BINDINGS_LIBRARIES ${JNI_LIBRARIES})
  include_directories(${JAVA_INCLUDE_PATH} ${JAVA_INCLUDE_PATH2} ${BINDINGS})
  set(JAVA_OUT_PATH ${LIBRARY_OUTPUT_PATH}/java/org/kipr/kovan)
  file(REMOVE_RECURSE ${JAVA_OUT_PATH})
  file(MAKE_DIRECTORY ${JAVA_OUT_PATH})
  add_custom_command(COMMAND ${SWIG_EXECUTABLE} -Wall -java -package org.kipr.kovan -c++ -I${INCLUDE} -outdir ${JAVA_OUT_PATH} -o ${CMAKE_BINARY_DIR}/kovan_wrap_java.cxx ${BINDINGS}/kovan.i
    DEPENDS ${SWIG_SOURCES}
    OUTPUT ${CMAKE_BINARY_DIR}/kovan_wrap_java.cxx)
endif()

add_library(kovan SHARED ${VI_SOURCES} ${VI_P_INCLUDES} ${OBJC_SOURCES} ${VF_INCLUDES} ${PIXELTOASTER_SOURCES} ${BINDINGS_SOURCES})

if(WIN32)
  # TODO: Make sure these are current
  target_link_libraries(kovan opencv_core2410 opencv_highgui2410 opencv_imgproc2410 libzbar-0 OpenNI2)
  target_link_libraries(kovan d3d9)
  target_link_libraries(kovan ws2_32)
else()
  target_link_libraries(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar avcodec avformat avutil z swscale bz2 OpenNI2)
endif()

target_link_libraries(kovan ${BINDINGS_LIBRARIES})

if(APPLE)
# set(CMAKE_OSX_SYSROOT "${OSX_DEVELOPER_ROOT}/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.7.sdk")
  find_library(OPENGL_LIBRARY OpenGL)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(CORESERVICE_LIBRARY CoreServices)
  target_link_libraries(kovan ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${CORESERVICE_LIBRARY})
endif()

if(KOVAN)
  target_link_libraries(kovan i2c_wrapper)
  add_definitions(-DKOVAN)
endif()

# add_definitions(-DARDRONE_DEBUG)

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
