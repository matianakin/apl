cmake_minimum_required(VERSION 3.21)
project(cpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp main.cpp ListOfNodes.cpp ListOfNodes.h ListOfConnections.cpp ListOfConnections.h ThreadHead.cpp ThreadHead.h)
