cmake_minimum_required(VERSION 3.0)

project(udp_test)

set (CMAKE_CXX_STANDARD 11)

set(SRC_CLIENT mcastclient.cpp)
set(SRC_MPORT_CLIENT mcastmportclient.cpp)
set(SRC_SERVER mcastserver.cpp)

add_executable(mcastclient ${SRC_CLIENT})
add_executable(mcastmportclient ${SRC_MPORT_CLIENT})
add_executable(mcastserver ${SRC_SERVER})