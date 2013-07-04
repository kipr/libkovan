ADD_EXECUTABLE(ardrone ardrone.cpp)
TARGET_LINK_LIBRARIES(ardrone kovan)

ADD_EXECUTABLE(ardrone_c ardrone.c)
TARGET_LINK_LIBRARIES(ardrone_c kovan)