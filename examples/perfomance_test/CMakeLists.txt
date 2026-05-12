add_executable(perfomance_test main.cpp)

target_link_libraries(perfomance_test PRIVATE GRAPHICS_LIB)

COPY_DLLS(perfomance_test)
