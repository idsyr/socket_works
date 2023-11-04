cmake_minimum_required(VERSION 3.5)

project(SocketCerver0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SocketCerver0 main.cpp)

install(TARGETS SocketCerver0
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
