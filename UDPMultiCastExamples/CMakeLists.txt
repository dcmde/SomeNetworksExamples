cmake_minimum_required(VERSION 3.0)

project(udp_test)

set (CMAKE_CXX_STANDARD 11)

set(SRC_CLIENT mcastlistener.cpp)
set(SRC_MPORT_CLIENT mcastmportlistener.cpp)
set(SRC_SERVER mcastpublisher.cpp)

add_executable(mcastlistener ${SRC_CLIENT})
add_executable(mcastmportlistener ${SRC_MPORT_CLIENT})
add_executable(mcastpublisher ${SRC_SERVER})
