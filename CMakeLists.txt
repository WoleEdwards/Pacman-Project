cmake_minimum_required(VERSION 3.23)
project(apsc143project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(apsc143project
        apsc143project.cpp
        colours.cpp
        colours.h)
