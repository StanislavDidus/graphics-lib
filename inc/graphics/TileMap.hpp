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
		TileMap() = default;
		TileMap(std::shared_ptr<Texture> texture,
			int world_width_tiles, int world_height_tiles,
			int tile_width_world, int tile_height_world,
			int tile_width_pixels, int tile_height_pixels);
		TileMap(std::shared_ptr<Texture> texture,
			int world_width_tiles, int world_height_tiles,
			int tile_width_world, int tile_height_world,
			int tile_width_pixels, int tile_height_pixels,
			int chunk_width_tile, int chunk_height_tile);
		~TileMap() = default;
		
		// Getters
		std::shared_ptr<Texture> getTexture() const { return texture;}
		const std::vector<Chunk>& getChunks() const { return chunks; }
		
		int getChunkWidthTiles() const { return chunk_width_tiles; };
		int getChunkHeightTiles() const { return chunk_height_tiles; };
		
		int getTileWidthWorld() const { return tile_width_world; }
		int getTileHeightWorld() const { return tile_height_world; }
		
		int getWorldWidthChunks() const { return world_width_tiles / chunk_width_tiles; }
		int getWorldHeightChunks() const { return world_height_tiles / chunk_height_tiles; }
		
		int getTileWidthPixels() const { return tile_width_world; }
		int getTileHeightPixels() const { return tile_height_world; }
	private:
		void create();
		
		std::vector<Chunk> chunks;
		std::shared_ptr<Texture> texture;
		int world_width_tiles = 0;
		int world_height_tiles = 0;
		int tile_width_world = 0;
		int tile_height_world = 0;
		int chunk_width_tiles = 0;
		int chunk_height_tiles = 0;
		int tile_width_pixels = 0;
		int tile_height_pixels = 0;
	};
}
