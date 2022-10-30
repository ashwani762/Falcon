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

set(APP_PROJECT_NAME "Falcon")
set(LIB_PROJECT_NAME "${APP_PROJECT_NAME}_lib")
set(LIB_GUI_PROJECT_NAME "${APP_PROJECT_NAME}_lib_gui")
set(LIB_UTILS_PROJECT_NAME "${APP_PROJECT_NAME}_lib_utils")

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

# QT 6

set (QT_MIN_VERSION "6.2.3")
find_package(Qt6 ${QT_MIN_VERSION} COMPONENTS Widgets Gui Core PrintSupport Network Svg REQUIRED)

if(Qt6Widgets_FOUND)
	message(STATUS "Found Qt6")

	# FIX: Qt was built with -reduce-relocations
	if (Qt6_POSITION_INDEPENDENT_CODE)
		SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
	endif()

	add_definitions (-DQT_DEPRECATED_WARNINGS)
endif()

# QT 6 Setup for Windows

if (WIN32)
	get_filename_component(QT_BINARY_DIR "${Qt6_DIR}" PATH)
	set(QT_LIB_DIR "${QT_BINARY_DIR}/../../bin")
	set(QT_PLUGINS_DIR "${QT_BINARY_DIR}/../../plugins")

	function(COPY_QT_BINARIES IS_DEBUG IS_APP)
		set(SUFFIX "")
		if (IS_DEBUG)
			set(SUFFIX "d")
		endif()

		set(CONFIGURATION "Release")
		if (IS_DEBUG)
			set(CONFIGURATION "Debug")
		endif()

		if(CMAKE_SIZEOF_VOID_P EQUAL 8)
			set(BITS "64")
		else()
			set(BITS "32")
		endif()

		set(TARGET "test")
		if (IS_APP)
			set(TARGET "app")

			file(GLOB MY_PUBLIC_HEADERS
				"${CMAKE_SOURCE_DIR}/setup/dynamic_libraries/win${BITS}/app/${CONFIGURATION}/*"
			)
			file(COPY ${MY_PUBLIC_HEADERS} DESTINATION "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/")

		endif()

		configure_file("${QT_LIB_DIR}/Qt6Core${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt6Core${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_LIB_DIR}/Qt6Gui${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt6Gui${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_LIB_DIR}/Qt6Network${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt6Network${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_LIB_DIR}/Qt6Svg${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt6Svg${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_LIB_DIR}/Qt6Widgets${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/Qt6Widgets${SUFFIX}.dll" COPYONLY)

		configure_file("${QT_PLUGINS_DIR}/platforms/qwindows${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/platforms/qwindows${SUFFIX}.dll" COPYONLY)

		configure_file("${QT_PLUGINS_DIR}/styles/qwindowsvistastyle${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/styles/qwindowsvistastyle${SUFFIX}.dll" COPYONLY)

		configure_file("${QT_PLUGINS_DIR}/imageformats/qgif${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qgif${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qicns${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qicns${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qico${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qico${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qjpeg${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qjpeg${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qsvg${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qsvg${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qtga${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qtga${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qtiff${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qtiff${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qwbmp${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qwbmp${SUFFIX}.dll" COPYONLY)
		configure_file("${QT_PLUGINS_DIR}/imageformats/qwebp${SUFFIX}.dll" "${CMAKE_BINARY_DIR}/${CONFIGURATION}/${TARGET}/imageformats/qwebp${SUFFIX}.dll" COPYONLY)
	endfunction(COPY_QT_BINARIES)

	COPY_QT_BINARIES(True True)
	COPY_QT_BINARIES(True False)
	COPY_QT_BINARIES(False True)
	COPY_QT_BINARIES(False False)

endif()

# JSON

set(JSON_BuildTests OFF CACHE INTERNAL "")

# setup

add_subdirectory("external/fmt" EXCLUDE_FROM_ALL)
add_subdirectory("external/imgui" EXCLUDE_FROM_ALL)
add_subdirectory("external/json" EXCLUDE_FROM_ALL)
add_subdirectory("external/sqlite3" EXCLUDE_FROM_ALL)
add_subdirectory("external/tinyxml2" EXCLUDE_FROM_ALL)


add_subdirectory ("src/app")
#add_subdirectory ("src/lib")
add_subdirectory ("src/lib_gui")
#add_subdirectory ("src/lib_utils")

# Lib

# add_library(${LIB_PROJECT_NAME} ${LIB_FILES})

# set_property(
# 	TARGET ${LIB_PROJECT_NAME}
# 	PROPERTY INCLUDE_DIRECTORIES
# 		"${LIB_INCLUDE_PATHS}"
# 		"${LIB_UTILS_INCLUDE_PATHS}"
# )

# target_include_directories(${LIB_PROJECT_NAME} SYSTEM
# 	PUBLIC ${Boost_INCLUDE_DIR}
# 			imgui
# 			nlohmann_json::nlohmann_json
# 			sqlite
# 			tinyxml

# )

# target_link_libraries(${LIB_PROJECT_NAME} PRIVATE
# 	${Boost_LIBRARIES}
# 	imgui
# 	nlohmann_json::nlohmann_json
# 	sqlite
# 	tinyxml
# )

# GUI

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

add_library(${LIB_GUI_PROJECT_NAME} ${LIB_GUI_FILES})

target_link_libraries(${LIB_GUI_PROJECT_NAME}
	# ${LIB_PROJECT_NAME}
	# ${LIB_UTILS_PROJECT_NAME}
	imgui
	Qt6::Widgets 
	Qt6::Core
	Qt6::Gui
	Qt6::Svg
	Qt6::Network
)

set_property(
	TARGET ${LIB_GUI_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${LIB_GUI_INCLUDE_PATHS}"
		"${CMAKE_BINARY_DIR}/src/lib_gui"
		# "${LIB_INCLUDE_PATHS}"
		# "${LIB_UTILS_INCLUDE_PATHS}"
)

target_include_directories(${LIB_GUI_PROJECT_NAME} SYSTEM
	PUBLIC imgui
)

set(CMAKE_AUTOMOC OFF)

# Utils

# add_library(${LIB_UTILS_PROJECT_NAME} ${LIB_UTILS_FILES})

# set_property(
# 	TARGET ${LIB_UTILS_PROJECT_NAME}
# 	PROPERTY INCLUDE_DIRECTORIES
# 		"${LIB_UTILS_INCLUDE_PATHS}"
# )

# target_include_directories(${LIB_UTILS_PROJECT_NAME} SYSTEM
# 	PUBLIC fmt
# )

# target_link_libraries(${LIB_UTILS_PROJECT_NAME} 
# 	fmt
# )


# App
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(${APP_PROJECT_NAME} ${APP_FILES})

target_link_libraries(
	${APP_PROJECT_NAME}
	# ${LIB_PROJECT_NAME}
	${LIB_GUI_PROJECT_NAME}
	# ${LIB_UTILS_PROJECT_NAME}
	nlohmann_json::nlohmann_json
)

set_property(
	TARGET ${APP_PROJECT_NAME}
	PROPERTY INCLUDE_DIRECTORIES
		"${APP_INCLUDE_PATHS}"
		"${LIB_GUI_INCLUDE_PATHS}"
		"${CMAKE_BINARY_DIR}/src/lib_gui"
		# "${LIB_INCLUDE_PATHS}"
		# "${LIB_UTILS_INCLUDE_PATHS}"
)

target_include_directories(${APP_PROJECT_NAME} SYSTEM
	PUBLIC ${Boost_INCLUDE_DIRS}
)