cmake_minimum_required(VERSION 3.15...4.2.1)

add_executable(tilemap main.cpp)

target_link_libraries(tilemap PRIVATE GRAPHICS_LIB)

graphics_setup_runtime_dependencies(tilemap)