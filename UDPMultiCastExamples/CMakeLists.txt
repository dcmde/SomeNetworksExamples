cmake_minimum_required(VERSION 3.0)

project(udp_test)

set (CMAKE_CXX_STANDARD 11)

set(SRC_CLIENT mcastclient.cpp)
set(SRC_UDP_CLIENT udpclient.c)
set(SRC_MPORT_CLIENT mcastmportclient.cpp)
set(SRC_SERVER mcastserver.cpp)
set(SRC_UDP_SERVER udpserver.c)

add_executable(mcastclient ${SRC_CLIENT})
add_executable(udpclient ${SRC_UDP_CLIENT})
add_executable(mcastmportclient ${SRC_MPORT_CLIENT})
add_executable(mcastserver ${SRC_SERVER})
add_executable(udpserver ${SRC_UDP_SERVER})
