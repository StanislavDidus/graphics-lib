#pragma once
#include "graphics/TileMap.hpp"
#include "graphics/SDLRenderer/Chunk.hpp"

namespace graphics
{
    class TileMapSDL : public TileMap
    {
    public:
        TileMapSDL() = default;
        TileMapSDL(std::shared_ptr<Texture> texture,
			WorldSize world_size,
			TileSize tile_size,
			TileSizePixels tile_size_pixels);
		TileMapSDL(std::shared_ptr<Texture> texture,
			WorldSize world_size,
			TileSize tile_size,
			TileSizePixels tile_size_pixels,
			ChunkSize chunk_size);
        ~TileMapSDL() override = default;
    	
    	void update() override;
    	
    	// Setters
    	void setGrid(int x, int y, const std::vector<int>& grid) override;
    	void setTile(int x, int y, int id) override;
    	
    	// Getters
    	const std::vector<Chunk>& getChunks() const { return chunks; }	
    private:
    	void create();
    	
        std::vector<Chunk> chunks; 
    };
}
