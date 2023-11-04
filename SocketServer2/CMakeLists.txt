cmake_minimum_required(VERSION 3.5)

project(SocketServer2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SocketServer2 main.cpp)

install(TARGETS SocketServer2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
