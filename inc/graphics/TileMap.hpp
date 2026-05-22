#pragma once

#include <memory>

#include <graphics/GpuTextureSDL.hpp>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include <graphics/Chunk.hpp>

namespace graphics
{
	class TileMap
	{
	public:
		TileMap(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<GpuTextureSDL> texture,
			int world_width_tiles, int world_height_tiles,
			int tile_width_world, int tile_height_world,
			int chunk_width_tile = 1, int chunk_height_tile = 1);
		~TileMap() = default;

		// Getters
		std::shared_ptr<GpuTextureSDL> getTexture() const;
		const std::vector<std::shared_ptr<Chunk>>& getChunks() const;
	private:
		std::shared_ptr<SDL_GPUDevice> device;
		std::shared_ptr<GpuTextureSDL> texture;
		
		std::vector<std::shared_ptr<Chunk>> chunks;
	};
}
