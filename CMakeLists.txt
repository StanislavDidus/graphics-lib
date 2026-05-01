cmake_minimum_required(VERSION 3.15...4.2.1)

project(GRAPHICS_LIB VERSION 1.0
	DESCRIPTION "Graphics library that uses SDL3 GPU API for rendering."
	LANGUAGES C CXX)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(extern)

set(SOURCE_FILES 
    "src/Renderer.cpp"
    "src/GpuRenderer.cpp"
    "src/Sprite.cpp"
    "src/Surface.cpp"
    "src/Window.cpp"
    "src/SpriteSheet.cpp" 
    "src/Color.cpp"
    "src/Font.cpp"
    "src/Text.cpp"
    "src/ViewportGuard.cpp"
    "src/SpriteAnimation.cpp"
    "src/ColorModGuard.cpp"
    "src/GpuShader.cpp"
    "src/GpuGraphicsPipeline.cpp"
    "src/GpuSampler.cpp"
    "src/TileMap.cpp"
    "src/RenderBatch.cpp"
    "src/Chunk.cpp"
    )

set(HEADER_FILES
    "inc/GpuRenderer.hpp"
    "inc/Sprite.hpp"
    "inc/Surface.hpp"
    "inc/Window.hpp"
    "inc/SpriteSheet.hpp"
    "inc/Color.hpp"
    "inc/Font.hpp"
    "inc/Text.hpp"
    "inc/ViewportGuard.hpp"
    "inc/SpriteAnimation.hpp"
    "inc/ColorModGuard.hpp"
    "inc/GpuShader.hpp"
    "inc/GpuGraphicsPipeline.hpp"
    "inc/Vertex.hpp"
    "inc/GpuBuffer.hpp"
    "inc/GpuTransferBuffer.hpp"
    "inc/GpuTexture.hpp"
    "inc/GpuRenderFunctions.hpp"
    "inc/GpuSampler.hpp"
    "inc/TileMap.hpp"
    "inc/CommandBuffer.hpp"
    "inc/RenderBatch.hpp"
    "inc/Chunk.hpp"
    )

set(ALL_FILES ${SOURCE_FILES} ${HEADER_FILES})

add_library(GRAPHICS_LIB ${ALL_FILES})

target_include_directories(GRAPHICS_LIB PUBLIC inc)

target_link_libraries(GRAPHICS_LIB PUBLIC 
	SDL3::SDL3-static 
	SDL3_ttf::SDL3_ttf-static 
	SDL3_shadercross::SDL3_shadercross-static 
	glm::glm imgui
)

target_compile_features(GRAPHICS_LIB PUBLIC cxx_std_23)