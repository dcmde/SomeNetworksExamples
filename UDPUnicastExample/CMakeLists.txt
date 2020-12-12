cmake_minimum_required(VERSION 3.0)

project(udp_test)

set (CMAKE_CXX_STANDARD 11)

set(SRC_UDP_CLIENT udppublisher.c)
set(SRC_UDP_SERVER udplistener.c)

add_executable(udppublisher ${SRC_UDP_CLIENT})
add_executable(udplistener ${SRC_UDP_SERVER})
