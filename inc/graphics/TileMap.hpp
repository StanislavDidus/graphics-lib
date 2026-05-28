#pragma once

#include <memory>

#include <graphics/GpuTextureSDL.hpp>
#include "TileMapData.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace graphics
{
	class TileMap
	{
	public:
		TileMap() = default;
		TileMap(std::shared_ptr<Texture> texture,
			WorldSize world_size,
			TileSize tile_size,
			TileSizePixels tile_size_pixels,
			ChunkSize chunk_size);
		virtual ~TileMap();
		
		virtual void update() = 0;
		
		// Setters
		virtual void setGrid(int x, int y, const std::vector<int>& grid) = 0;
		virtual void setTile(int x, int y, int id) = 0;
		
		// Getters
		std::shared_ptr<Texture> getTexture() const { return texture; }
		WorldSize getWorldSize() const { return world_size; }
		TileSize getTilesize() const { return tile_size; }
		TileSizePixels getTileSizePixels() const { return tile_size_pixels; }
		ChunkSize getChunkSize() const { return chunk_size; }
	protected:
		//std::vector<Chunk> chunks;
		std::shared_ptr<Texture> texture;
		
		WorldSize world_size;
		TileSize tile_size;
		TileSizePixels tile_size_pixels;
		ChunkSize chunk_size;
	};
}
