#pragma once

#include <array>
#include <memory>
#include <variant>

#include <graphics/TileMap.hpp>
#include <graphics/GpuTextureSDL.hpp>
#include "glm/vec4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "graphics/GPURenderer/Chunk.hpp"

namespace graphics
{
	struct Vertex
	{
		float x, y, z;
		float r, g, b, a;
	};
	struct TextureVertex
	{
		float x, y, z;
		float u, v;
	};
	struct TextureObject
	{
		std::string texture_name;
		std::array<TextureVertex, 4> vertices;
	};
	struct RectangleData
	{
		glm::vec2 position;
		glm::vec2 size;
		glm::vec4 color;
	};
	struct LineData
	{
		float x1;
		float y1;
		float x2;
		float y2;
		glm::vec4 color;
	};
	struct TileMapData
	{
		std::shared_ptr<TileMap> tile_map;
	};
	struct ChunkData
	{
		std::shared_ptr<Chunk> chunk;
		glm::vec3 position;
	};
	struct alignas(16) SpriteData
	{
		glm::vec4 pos_rot; // Position3 and Rotation1
		glm::vec4 size; //Size2 Padding2
		glm::vec4 uv; // UV4
		glm::vec4 color; // Color4
		glm::vec4 flip; // Flip1 Padding3
	};
	struct alignas(16) GpuSprite
	{
		SDL_GPUTexture* texture;
		SDL_GPUSampler* sampler;
		SpriteData data;
	};

	using DrawObject = std::variant<GpuSprite, RectangleData, LineData, ChunkData>;

	struct alignas(16) SpriteUniform
	{
		uint32_t index;
	};

	struct ScreenSize
	{
		int width;
		int height;
	};

}
