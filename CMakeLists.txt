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
	include_directories(${DIRECTX}/Include)
endif()

##################################
# Windows kiss-platform nonsense #
##################################
if(WIN32)
	include_directories(${CMAKE_SOURCE_DIR}/../prefix/include)
	include_directories(${CMAKE_SOURCE_DIR}/../opencv/kiss-prefix/include)
	include_directories(${CMAKE_SOURCE_DIR}/../zbar-0.10/prefix/include)
	link_directories(${CMAKE_SOURCE_DIR}/../opencv/kiss-prefix/lib)
	link_directories(${CMAKE_SOURCE_DIR}/../lib)
	link_directories(${CMAKE_SOURCE_DIR}/../zbar-0.10/prefix/lib)
endif()

if(APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
elseif(WIN32)
	set(SOURCES ${SOURCES} ${CMAKE_SOURCE_DIR}/compat/mingw.c)
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif()

# add_definitions(-O3)

add_library(kovan SHARED ${SOURCES} ${OBJC_SOURCES})
if(NOT WIN32)
	target_link_libraries(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar avcodec avformat avutil z swscale bz2)
else()
	# TODO: Make sure these are current
	target_link_libraries(kovan pthread opencv_core249 opencv_highgui249 opencv_imgproc249 zbar)
	link_directories(${DIRECTX})
	target_link_libraries(kovan d3d9)
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

