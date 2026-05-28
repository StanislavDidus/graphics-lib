#include "TilemapSDL.hpp"
#include "graphics/TileMap.hpp"

namespace graphics
{
    TileMapSDL::TileMapSDL(std::shared_ptr<Texture> texture, WorldSize world_size, TileSize tile_size,
        TileSizePixels tile_size_pixels)
            : TileMap(texture, world_size, tile_size, tile_size_pixels, ChunkSize(world_size.width, world_size.height))
    {
        create();
    }

    TileMapSDL::TileMapSDL(std::shared_ptr<Texture> texture, WorldSize world_size, TileSize tile_size,
        TileSizePixels tile_size_pixels, ChunkSize chunk_size)
            : TileMap(texture, world_size, tile_size, tile_size_pixels, chunk_size)
    {
        create();
    }

    void TileMapSDL::update()
    {
       for (auto& chunk : chunks)
       {
           chunk.update();
       }
    }

    void TileMapSDL::setGrid(int x, int y, const std::vector<int>& grid)
    {
        int chunk_x = x / chunk_size.width;
        int chunk_y = y / chunk_size.height;
        
        int world_width_chunks = world_size.width / chunk_size.width;
        chunks[chunk_y + chunk_x * world_width_chunks].setGrid(grid);
    }

    void TileMapSDL::setTile(int x, int y, int id)
    {  
        int chunk_x = x / chunk_size.width;
        int chunk_y = y / chunk_size.height;
        
        int world_width_chunks = world_size.width / chunk_size.width;
        int world_height_chunks = world_size.height / chunk_size.height;
        
        int offset_x = x % chunk_size.width;
        int offset_y = y % chunk_size.height;
        chunks[chunk_y + chunk_x * world_height_chunks].setTile(offset_x, offset_y, id);
    }

    void TileMapSDL::create()
    {
        // Empty grid
        std::vector<int> grid;
        int chunk_size_tiles = chunk_size.width * chunk_size.height;
        grid.reserve(chunk_size_tiles);
        for (int i = 0; i < chunk_size_tiles; ++i) grid.emplace_back(0);
    
        int world_width_chunks = world_size.width / chunk_size.width;
        int world_height_chunks = world_size.height / chunk_size.height;
    
        // Column major order
        for (int y = 0; y < world_height_chunks; ++y)
        {
            for (int x = 0; x < world_width_chunks; ++x)
            {
                chunks.emplace_back(chunk_size, grid);
            }
        }
    }
}
