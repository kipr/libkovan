#################################
#   libkovan CMake Build File   #
#################################

PROJECT(libkovan)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

SET(INCLUDE ${libkovan_SOURCE_DIR}/include)
SET(SRC ${libkovan_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(${INCLUDE})

FILE(GLOB INCLUDES ${INCLUDE}/kovan/*.h ${INCLUDE}/kovan/*.hpp)
FILE(GLOB SOURCES ${SRC}/*.cpp)

SET(CMAKE_CXX_FLAGS "-Wall")

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

IF(WIN32)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../prefix/include)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../opencv/kiss-prefix/include)
	INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../zbar-0.10/prefix/include)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../opencv/kiss-prefix/lib)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../lib)
	LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/../zbar-0.10/prefix/lib)
ENDIF(WIN32)

IF(APPLE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
ELSEIF(WIN32)
	SET(CMAKE_CXX_FLAGS "-Wl,--enable-auto-import")
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

add_definitions(-O3)

ADD_LIBRARY(kovan SHARED ${SOURCES})
IF(NOT WIN32)
TARGET_LINK_LIBRARIES(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar avcodec avformat avutil z swscale bz2)
ELSE(NOT WIN32)
# TODO: Make sure these are current
TARGET_LINK_LIBRARIES(kovan pthread opencv_core249 opencv_highgui249 opencv_imgproc249 zbar)
ENDIF(NOT WIN32)

IF(KOVAN)
	TARGET_LINK_LIBRARIES(kovan i2c_wrapper)
	ADD_DEFINITIONS(-DKOVAN)
ENDIF(KOVAN)

# ADD_DEFINITIONS(-DARDRONE_DEBUG)

IF(WIN32)
target_link_libraries(kovan ws2_32)
install(FILES ${INCLUDES} DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/include/kovan)
INSTALL(TARGETS kovan
	ARCHIVE DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib
	RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/../prefix/lib)
ELSE(WIN32)
install(FILES ${INCLUDES} DESTINATION include/kovan)
install(TARGETS kovan LIBRARY DESTINATION lib)
ENDIF(WIN32)

add_subdirectory(test)
