cmake_minimum_required(VERSION 3.15...4.2.1)

add_executable(empty_window main.cpp)

target_link_libraries(empty_window PRIVATE GRAPHICS_LIB)

graphics_setup_runtime_dependencies(empty_window "../../")