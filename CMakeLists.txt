# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

set(APP_PROJECT_NAME "Experiment2")
set(LIB_PROJECT_NAME "${PROJECT_NAME}_lib")
set(LIB_GUI_PROJECT_NAME "${PROJECT_NAME}_lib_gui")

project(${APP_PROJECT_NAME})

include(cmake/add_files.cmake)

# prohibit in-source-builds
if (${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	message(STATUS "In-source-builds are not allowed")
	message(STATUS "Clean your source directory (e.g. delete the CMakeCache.txt file)")
	message(FATAL_ERROR "Please create a separate build directory and call CMake again")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

# Threads 
find_package(Threads REQUIRED)

# Options
option(BUILD_TEST      "Build tests" OFF)
option(BUILD_BENCH     "Build benchmarks" OFF)

if(BUILD_BENCH)
	# For Benchmark
	set(BENCHMARK_DOWNLOAD_DEPENDENCIES ON)

	# Include sub-projects.
	add_subdirectory("external/benchmark")
endif()

if(BUILD_TEST)
  enable_testing()
  add_subdirectory("external/googletest")
endif()

# Settings

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-warning-option -fcolor-diagnostics -fvisibility-inlines-hidden")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	ADD_COMPILE_OPTIONS(/MP)
endif()

# Boost 

set(Boost_USE_MULTITHREAD ON)
set(Boost_USE_STATIC_LIBS ON CACHE BOOL "Set to ON to force the use of the static libraries.")
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.79.0 COMPONENTS system program_options filesystem date_time REQUIRED)

# setup

add_subdirectory("external/fmt" EXCLUDE_FROM_ALL)
add_subdirectory("external/imgui" EXCLUDE_FROM_ALL)

add_subdirectory ("src/app")
add_subdirectory ("src/lib")


# Lib

add_library(${LIB_PROJECT_NAME} ${LIB_FILES})

set_property(
	TARGET ${LIB_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${LIB_INCLUDE_PATHS}"
)

target_include_directories(${LIB_PROJECT_NAME} SYSTEM
	PUBLIC ${Boost_INCLUDE_DIR}
)

target_link_libraries(${LIB_PROJECT_NAME} ${Boost_LIBRARIES})

# App

add_executable(${APP_PROJECT_NAME} ${APP_FILES})

target_link_libraries(
	${APP_PROJECT_NAME}
	${LIB_PROJECT_NAME}
	imgui
)

set_property(
	TARGET ${APP_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${APP_INCLUDE_PATHS}"
		"${LIB_INCLUDE_PATHS}"
)

target_include_directories(${APP_PROJECT_NAME} SYSTEM
	PUBLIC ${Boost_INCLUDE_DIRS}
	imgui
)