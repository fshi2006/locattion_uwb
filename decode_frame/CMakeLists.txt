cmake_minimum_required(VERSION 2.8.3)
project(decodeframe)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
set(catkin_deps
  sensor_msgs
  roscpp
  serial
  geometry_msgs
  )
find_package(catkin REQUIRED COMPONENTS
  ${catkin_deps}
)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES simple_car
  CATKIN_DEPENDS ${catkin_deps}
#  DEPENDS system_lib
)


## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(${PROJECT_NAME}_node
  src/decode_frame.cpp
  #src/serial_port.cpp
  src/trilateration_core.cpp
)

target_link_libraries(${PROJECT_NAME}_node
${catkin_LIBRARIES})

add_executable(${PROJECT_NAME}_listener_node
  src/listener.cpp
)

target_link_libraries(${PROJECT_NAME}_listener_node
${catkin_LIBRARIES})
