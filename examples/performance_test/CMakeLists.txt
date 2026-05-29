cmake_minimum_required(VERSION 3.15...4.2.1)

add_executable(performance_test main.cpp)

target_link_libraries(performance_test PRIVATE GRAPHICS_LIB)

graphics_setup_runtime_dependencies(performance_test "../../")