set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(netanomy-master main.cpp logger.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(ZMQ_INCLUDE ${ZMQ_INCLUDE} CACHE STRING "Location of ZeroMQ headers.")
SET(ZMQ_LIBRARY ${ZMQ_LIBRARY} CACHE STRING "ZeroMQ libraries to link in.")

include_directories(${ZMQ_INCLUDE})
target_link_libraries(netanomy-master ${ZMQ_LIBRARY})
