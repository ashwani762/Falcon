cmake_minimum_required(VERSION 3.20)
project(tinyxml)

set(CMAKE_CXX_STANDARD 17)

add_library(tinyxml OBJECT
    tinyxml2.cpp
)

target_include_directories(tinyxml PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)