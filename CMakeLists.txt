#################################
#   libkovan CMake Build File   #
#################################

project(libkovan)

cmake_minimum_required(VERSION 2.8.0)

set(INCLUDE ${libkovan_SOURCE_DIR}/include)
set(SRC ${libkovan_SOURCE_DIR}/src)
set(PIXELTOASTER ${SRC}/pixeltoaster)

include_directories(${INCLUDE})

file(GLOB INCLUDES ${INCLUDE}/kovan/*.h ${INCLUDE}/kovan/*.hpp)
file(GLOB SOURCES ${SRC}/*.cpp ${PIXELTOASTER}/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

#############################
#   PixelToaster settings   #
#############################
if(APPLE)
	file(GLOB OBJC_SOURCES ${PIXELTOASTER}/*.mm)
elseif(WIN32)
	add_definitions(-fno-check-new)
endif()

##################################
# Windows kiss-platform nonsense #
##################################
if(WIN32)
  set(OPENCV_INSTALL_DIR ${CMAKE_SOURCE_DIR}/../opencv/build/install)
  set(ZBAR_INSTALL_DIR "C:/Program Files (x86)/ZBar")
  set(OpenNI2_INSTALL_DIR "C:/Program Files (x86)/OpenNI2")

  include_directories(${CMAKE_SOURCE_DIR}/../prefix/include)
  include_directories(${OPENCV_INSTALL_DIR}/include)
  include_directories(${ZBAR_INSTALL_DIR}/include)
  include_directories(${OpenNI2_INSTALL_DIR}/Include)
  link_directories(${CMAKE_SOURCE_DIR}/../prefix/lib)
  link_directories(${OPENCV_INSTALL_DIR}/x86/vc11/lib)
  link_directories(${ZBAR_INSTALL_DIR}/lib)
  
else()
  set(OpenNI2_INSTALL_DIR "${CMAKE_SOURCE_DIR}/../OpenNI2")
  
endif()

include_directories(${OpenNI2_INSTALL_DIR}/Include)
if(APPLE)
  link_directories(${OpenNI2_INSTALL_DIR}/Bin/x64-Release)
elseif(WIN32)
  link_directories(${OpenNI2_INSTALL_DIR}/Lib)
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

add_library(kovan SHARED ${SOURCES} ${OBJC_SOURCES} ${INCLUDES})
if(WIN32)
	# TODO: Make sure these are current
	target_link_libraries(kovan opencv_core300 opencv_highgui300 opencv_imgproc300 libzbar-0 OpenNI2)
	target_link_libraries(kovan d3d9)
else()
  target_link_libraries(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar avcodec avformat avutil z swscale bz2 OpenNI2)
endif()

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

if(WIN32)
target_link_libraries(kovan ws2_32)
install(FILES ${INCLUDES} DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/include/kovan)
install(TARGETS kovan
  ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib
  RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib)
else()
install(FILES ${INCLUDES} DESTINATION include/kovan)
install(TARGETS kovan LIBRARY DESTINATION lib)
endif()

option(BUILD_DOCUMENTATION "Use Doxygen to create the HTML based API documentation" OFF)
if(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen is needed to build the documentation. Please install it correctly")
  endif()
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

add_executable(test_depth ${CMAKE_SOURCE_DIR}/test.cpp)
target_link_libraries(test_depth kovan)


