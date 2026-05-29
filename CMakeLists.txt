cmake_minimum_required(VERSION 3.15...4.2.1)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(GRAPHICS_BUILD_SAMPLES "Build samples" ON)

set(GRAPHICS_VERSION_MAJOR 0)
set(GRAPHICS_VERSION_MINOR 0)
set(GRAPHICS_VERSION_PATCH 1)
set(GRAPHICS_VERSION
	${GRAPHICS_VERSION_MAJOR}.${GRAPHICS_VERSION_MINOR}.${GRAPHICS_VERSION_PATCH})

add_compile_definitions(WEB)

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(GRAPHICS_LIB VERSION ${GRAPHICS_VERSION}
	DESCRIPTION "Simple graphics library that uses SDL3 for rendering"
	LANGUAGES C CXX)

# Enable multi-processor builds in MSVC.
if(MSVC)
	add_compile_options(/MP)
endif(MSVC)

add_subdirectory(extern EXCLUDE_FROM_ALL)

set(SOURCE_FILES 
    "src/graphics/exception.cpp"
    "src/graphics/Sprite.cpp"
    "src/graphics/Surface.cpp"
    "src/graphics/Window.cpp"
    "src/graphics/SpriteSheet.cpp"
    "src/graphics/Color.cpp"
    "src/graphics/Font.cpp"
    "src/graphics/Text.cpp"
    "src/graphics/SpriteAnimation.cpp"
    "src/graphics/GpuShader.cpp"
    "src/graphics/GpuGraphicsPipeline.cpp"
    "src/graphics/GpuSampler.cpp"
    "src/graphics/TileMap.cpp"
    "src/graphics/RenderBatch.cpp"
	"src/graphics/RendererImpl.cpp"
	"src/graphics/Renderer.cpp"
	"src/graphics/GPURenderer/RendererImplGPU.cpp"
	"src/graphics/TextureSDL.cpp"
	"src/graphics/GpuTextureSDL.cpp" 
		src/graphics/SDLRenderer/RendererImplSDL.cpp
        src/graphics/SDLRenderer.cpp
		src/graphics/TextEngine.cpp
		src/graphics/SDLRenderer/TilemapSDL.cpp
		src/graphics/GPURenderer/GpuTilemapSDL.cpp
		src/graphics/SDLRenderer/Chunk.cpp
		src/graphics/GPURenderer/Chunk.cpp
		src/graphics/GPURenderer/WindowClaimer.cpp
)

set(HEADER_FILES
		"inc/graphics/Renderer.hpp"
		"inc/graphics/graphics.hpp"
		"inc/graphics/exception.hpp"
		"inc/graphics/Sprite.hpp"
		"inc/graphics/Surface.hpp"
		"inc/graphics/Window.hpp"
		"inc/graphics/SpriteSheet.hpp"
		"inc/graphics/Color.hpp"
		"inc/graphics/Font.hpp"
		"inc/graphics/Text.hpp"
		"inc/graphics/SpriteAnimation.hpp"
		"inc/graphics/GpuShader.hpp"
		"inc/graphics/GpuGraphicsPipeline.hpp"
		"inc/graphics/Vertex.hpp"
		"inc/graphics/GpuBuffer.hpp"
		"inc/graphics/GpuTransferBuffer.hpp"
		"inc/graphics/GpuTextureSDL.hpp"
		"inc/graphics/GpuSampler.hpp"
		"inc/graphics/TileMap.hpp"
		"inc/graphics/CommandBuffer.hpp"
		"inc/graphics/RenderBatch.hpp"
		"inc/graphics/BatchHelpers.hpp"
		"inc/graphics/Enums.hpp"
		"src/graphics/RendererImpl.hpp"
		"src/graphics/GPURenderer/RendererImplGPU.hpp"
		"inc/graphics/Texture.hpp"
		"inc/graphics/TextureSDL.hpp"
		src/graphics/SDLRenderer/RendererImplSDL.hpp
		src/graphics/SDLRenderer.hpp
		inc/graphics/TextEngine.hpp
		src/graphics/SDLRenderer/TilemapSDL.hpp
		src/graphics/GPURenderer/GpuTilemapSDL.hpp
		src/graphics/SDLRenderer/Chunk.hpp
		src/graphics/GPURenderer/Chunk.hpp
		inc/graphics/TileMapData.hpp
		src/graphics/GPURenderer/WindowClaimer.hpp
		inc/graphics/config.hpp
)

set(ALL_FILES ${SOURCE_FILES} ${HEADER_FILES})

add_library(GRAPHICS_LIB STATIC ${ALL_FILES})
add_library(GRAPHICS_LIB::graphics ALIAS GRAPHICS_LIB)

add_custom_target(copy_assets
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(GRAPHICS_LIB copy_assets)

function(graphics_setup_runtime_dependencies PROJECT_BIN ASSET_ROOT)
    add_custom_command(TARGET ${PROJECT_BIN} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${ASSET_ROOT}/libs"
    $<TARGET_FILE_DIR:${PROJECT_BIN}>
    COMMENT "Copying dlls to build directory ${PROJECT_BIN}")
	
	add_custom_command(TARGET ${PROJECT_BIN} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${ASSET_ROOT}/shaders/"
	$<TARGET_FILE_DIR:${PROJECT_BIN}>/shaders
	COMMENT "Copying shaders to build directory ${PROJECT_BIN}")
endfunction()

#Copy lib folder directly to bin folder

target_include_directories(GRAPHICS_LIB 
		PUBLIC inc
		PRIVATE src
)

target_link_libraries(GRAPHICS_LIB PUBLIC 
		SDL3::SDL3-static 
		SDL3_ttf::SDL3_ttf-static 
		SDL3_shadercross::SDL3_shadercross-static 
		glm::glm
)

if(GRAPHICS_BUILD_SAMPLES)
	add_subdirectory(examples)
endif(GRAPHICS_BUILD_SAMPLES)

target_compile_features(GRAPHICS_LIB PUBLIC cxx_std_23)