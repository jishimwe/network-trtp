cmake_minimum_required(VERSION 3.16)
project(Network C)

set(CMAKE_C_STANDARD 99)
find_package(ZLIB REQUIRED)
add_executable(Network entity/log.h entity/log.c entity/packet_interface.h entity/packet_interface.c entity/main.c entity/receiver.h entity/receiver.c entity/sender.c entity/sender.h entity/linked_list.c entity/socket_helper.c entity/socket_helper.h)
target_link_libraries(Network PRIVATE ZLIB::ZLIB)