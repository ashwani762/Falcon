cmake_minimum_required (VERSION 3.20)

project (binary_to_compressed_c)
add_executable(binary_to_compressed_c temp/tools/binary_to_compressed_c.cpp)
install (TARGETS binary_to_compressed_c RUNTIME DESTINATION ".")