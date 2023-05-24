cmake_minimum_required(VERSION 3.10)
project(blue_ping1d VERSION 0.1)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    std_msgs
    message_generation
    # dynamic_reconfigure
)
# generate_dynamic_reconfigure_options(${CMAKE_CURRENT_SOURCE_DIR}/cfg/OculusSonar.cfg)

add_message_files(FILES
    Profile.msg
)
generate_messages(DEPENDENCIES
    std_msgs
)

catkin_package(CATKIN_DEPENDS roscpp rospy std_msgs)

# replace with target include directories ?
include_directories(${catkin_INCLUDE_DIRS})

if(NOT TARGET ping_protocol)
    find_package(ping_protocol REQUIRED)
endif()

add_executable(blue_ping1d_node
    src/blue_ping1d_node.cpp
)
target_include_directories(blue_ping1d_node PRIVATE src)
target_link_libraries(blue_ping1d_node PRIVATE
    ${catkin_LIBRARIES}
    ping_protocol
)
# add_dependencies(blue_ping1d_node blue_ping1d_generate_messages_cpp ${PROJECT_NAME}_gencfg)
add_dependencies(blue_ping1d_node blue_ping1d_generate_messages_cpp)

