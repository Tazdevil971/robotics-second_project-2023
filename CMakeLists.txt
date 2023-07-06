cmake_minimum_required(VERSION 3.0.2)
project(second_project)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  tf
  actionlib
)

generate_messages(DEPENDENCIES std_msgs)

catkin_package(CATKIN_DEPENDS std_msgs)

include_directories(tf_publisher ${catkin_INCLUDE_DIRS})
add_executable(tf_publisher src/tf_publisher.cpp)
target_link_libraries(tf_publisher ${catkin_LIBRARIES})

include_directories(navigation ${catkin_INCLUDE_DIRS})
add_executable(navigation src/navigation.cpp)
target_link_libraries(navigation ${catkin_LIBRARIES})