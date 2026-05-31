cmake_minimum_required(VERSION 3.15...4.2.1)

set(SDLTTF_VENDORED ON)
set(SDLSHADERCROSS_SPIRVCROSS_SHARED OFF CACHE BOOL "Build Spirvcross shared" FORCE)
set(SDLSHADERCROSS_VENDORED ON)

add_subdirectory(SDL EXCLUDE_FROM_ALL)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)
if(NOT GRAPHICS_USE_SDL_RENDERER)
    add_subdirectory(SDL_shadercross EXCLUDE_FROM_ALL)
endif()
add_subdirectory(glm EXCLUDE_FROM_ALL)

set(SDL_TARGETS
        SDL_uclibc SDL3-static SDL3-shared
        SDL3_ttf SDL3_ttf-static
        freetype harfbuzz plutosvg plutovg
        SDL3_shadercross SDL3_shadercross-static
)

if(NOT GRAPHICS_USE_SDL_RENDERER)
    list(APPEND SDL_TARGETS
        SDL3_shadercross 
        SDL3_shadercross-static
    )
endif()

set(GLM_TARGETS
        glm glm_shared
)

macro(set_targets_folder TARGETS FOLDER)
    foreach (target ${TARGETS})
        if(TARGET ${target})
            set_target_properties(${target} PROPERTIES FOLDER ${FOLDER})
        endif ()
    endforeach ()
endmacro()

set_targets_folder("${GLM_TARGETS}" "extern/glm")
set_targets_folder("${SDL_TARGETS}" "extern/SDL")
