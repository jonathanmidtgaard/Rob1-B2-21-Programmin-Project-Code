cmake_minimum_required(VERSION 2.8.3)
project(program)

find_package(catkin REQUIRED roscpp)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES program
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

include_directories(
# include
${catkin_INCLUDE_DIRS}
)

add_executable(game src/maingame.cpp)

target_link_libraries(game
${catkin_LIBRARIES}
)

add_executable(Score_list src/scores.cpp)

target_link_libraries(Score_list
${catkin_LIBRARIES}
)
