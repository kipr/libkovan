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

IF(APPLE)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -arch x86_64 -g")
ELSEIF(WIN32)
	SET(CMAKE_CXX_FLAGS "-Wl,--enable-auto-import")
ELSEIF(UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
ENDIF()

ADD_LIBRARY(kovan SHARED ${SOURCES})
TARGET_LINK_LIBRARIES(kovan pthread opencv_core opencv_highgui opencv_imgproc zbar)
IF(KOVAN)
	TARGET_LINK_LIBRARIES(kovan i2c_wrapper)
	ADD_DEFINITIONS(-DKOVAN)
ENDIF(KOVAN)

install(FILES ${INCLUDES} DESTINATION include/kovan)
INSTALL(TARGETS kovan LIBRARY DESTINATION lib)

add_subdirectory(test)