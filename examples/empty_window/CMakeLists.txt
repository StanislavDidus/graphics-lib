add_executable(empty_window main.cpp)

target_link_libraries(empty_window PRIVATE GRAPHICS_LIB)

if(WIN32)
COPY_DLLS(empty_window)
endif(WIN32)