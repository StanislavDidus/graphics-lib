#SDL
set(SDL_X11_XFIXES OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Don't build SDL shared" FORCE)
set(SDL_STATIC ON CACHE BOOL "Build SDL static" FORCE)
add_subdirectory(SDL EXCLUDE_FROM_ALL)

#SDL_ttf
set(SDLTTF_VENDORED ON)
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)

#SDL_shadercross
set(SDLSHADERCROSS_SHARED OFF CACHE BOOL "Don't build SDL_shadecross shared" FORCE)
set(SDLSHADERCROSS_STATIC ON CACHE BOOL "Build SDL_shadecross static" FORCE)
set(SDLSHADERCROSS_SPIRVCROSS_SHARED OFF CACHE BOOL "Build Spirvcross shared" FORCE)
set(SDLSHADERCROSS_DXC ON CACHE BOOL "Enable DXC compilation" FORCE)
set(SDLSHADERCROSS_VENDORED ON)
add_subdirectory(SDL_shadercross EXCLUDE_FROM_ALL)

#glm
add_subdirectory(glm EXCLUDE_FROM_ALL)