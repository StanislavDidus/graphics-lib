cmake_minimum_required(VERSION 3.15...4.2.1)

project(GRAPHICS_LIB VERSION 1.0
	DESCRIPTION "Graphics library that uses SDL3 GPU API for rendering."
	LANGUAGES C CXX)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(extern)

set(SOURCE_FILES 
    "src/GpuRenderer.cpp"
    "src/exception.cpp"
    "src/Sprite.cpp"
    "src/Surface.cpp"
    "src/Window.cpp"
    "src/SpriteSheet.cpp" 
    "src/Color.cpp"
    "src/Font.cpp"
    "src/Text.cpp"
    "src/SpriteAnimation.cpp"
    "src/GpuShader.cpp"
    "src/GpuGraphicsPipeline.cpp"
    "src/GpuSampler.cpp"
    "src/TileMap.cpp"
    "src/RenderBatch.cpp"
    "src/Chunk.cpp"
    )

set(HEADER_FILES
	"inc/graphics/graphics.hpp"
	"inc/graphics/exception.hpp"
    "inc/graphics/GpuRenderer.hpp"
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
    "inc/graphics/GpuTexture.hpp"
    "inc/graphics/GpuRenderFunctions.hpp"
    "inc/graphics/GpuSampler.hpp"
    "inc/graphics/TileMap.hpp"
    "inc/graphics/CommandBuffer.hpp"
    "inc/graphics/RenderBatch.hpp"
    "inc/graphics/Chunk.hpp"
    )

set(ALL_FILES ${SOURCE_FILES} ${HEADER_FILES})

add_library(GRAPHICS_LIB ${ALL_FILES})

add_custom_target(copy_assets
		COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)
add_dependencies(GRAPHICS_LIB copy_assets)

target_include_directories(GRAPHICS_LIB 
		PUBLIC inc
		PRIVATE src
)

target_link_libraries(GRAPHICS_LIB PUBLIC 
	SDL3::SDL3-static 
	SDL3_ttf::SDL3_ttf-static 
	SDL3_shadercross::SDL3_shadercross-static 
	glm::glm glm
)

target_compile_features(GRAPHICS_LIB PUBLIC cxx_std_23)

add_subdirectory(examples)
