cmake_minimum_required(VERSION 3.5)

project(TCP_socket_client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TCP_socket_client main.cpp)

install(TARGETS TCP_socket_client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
