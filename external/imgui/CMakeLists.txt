cmake_minimum_required(VERSION 3.20)
project(imgui)

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    find_package(PkgConfig REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif()

if(UNIX)
    find_package(OpenGL REQUIRED)
endif()

if(WIN32)
    find_package(glfw3 CONFIG REQUIRED)
    find_package(freetype CONFIG REQUIRED)
endif()

add_library(imgui OBJECT
    source/imgui.cpp
    source/imgui_draw.cpp
    source/imgui_freetype.cpp
    source/imgui_impl_glfw.cpp
    source/imgui_impl_opengl3.cpp
    source/imgui_tables.cpp
    source/imgui_widgets.cpp

    source/imnodes.cpp

    source/implot.cpp
    source/implot_items.cpp

    source/fonts/font_codicon.cpp
    source/fonts/font_fa.cpp
    source/fonts/font_opensans.cpp
)

add_compile_definitions(IMGUI_IMPL_OPENGL_LOADER_GLAD)

target_include_directories(imgui PUBLIC include ${FREETYPE_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})

target_link_directories(imgui PUBLIC ${GLFW_LIBRARY_DIRS})

if (WIN32)
    target_link_libraries(imgui PUBLIC freetype glfw opengl32.lib)
elseif (UNIX)
    target_link_libraries(imgui PUBLIC Freetype::Freetype glfw OpenGL::GL)
endif()

