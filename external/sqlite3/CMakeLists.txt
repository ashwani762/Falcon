cmake_minimum_required(VERSION 3.20)
project(sqlite)

add_compile_definitions(SQLITE_ENABLE_FTS5)
add_compile_definitions(SQLITE_ENABLE_GEOPOLY)
add_compile_definitions(SQLITE_ENABLE_SESSION)

add_library(sqlite OBJECT
    sqlite3.c
)

target_include_directories(sqlite PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
