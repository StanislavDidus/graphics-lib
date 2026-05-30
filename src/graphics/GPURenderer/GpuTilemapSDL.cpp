#include "GpuTilemapSDL.hpp"

#include "graphics/TileMap.hpp"

namespace graphics
{
    GpuTilemapSDL::GpuTilemapSDL(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<Texture> texture, WorldSize world_size, TileSize tile_size,
        TileSizePixels tile_size_pixels)
            : TileMap(texture, world_size, tile_size, tile_size_pixels, ChunkSize(world_size.width, world_size.height)), device(device)
    {
        create();
    }

    GpuTilemapSDL::GpuTilemapSDL(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<Texture> texture, WorldSize world_size, TileSize tile_size,
                                 TileSizePixels tile_size_pixels, ChunkSize chunk_size)
            : TileMap(texture, world_size, tile_size, tile_size_pixels, chunk_size), device(device)
    {
        create();
    }

    void GpuTilemapSDL::update()
    {
        for (auto& chunk : chunks)
        {
            chunk->update();
        }
    }

    void GpuTilemapSDL::setGrid(int x, int y, const std::vector<int>& grid)
    {
        int chunk_x = x / chunk_size.width;
        int chunk_y = y / chunk_size.height;
        
        int world_width_chunks = world_size.width / chunk_size.width;
        chunks[chunk_y + chunk_x * world_width_chunks]->setGrid(grid);
    }

    void GpuTilemapSDL::setTile(int x, int y, int id)
    {
        int chunk_x = x / chunk_size.width;
        int chunk_y = y / chunk_size.height;
        
        int world_width_chunks = world_size.width / chunk_size.width;
        int world_height_chunks = world_size.height / chunk_size.height;
        
        int offset_x = x % chunk_size.width;
        int offset_y = y % chunk_size.height;
        chunks[chunk_y + chunk_x * world_height_chunks]->setTile(offset_x, offset_y, id);
    }

    void GpuTilemapSDL::create()
    {
        int world_width_chunks = world_size.width / chunk_size.width;
        int world_height_chunks = world_size.height / chunk_size.height;
	
        int index = 0;
        chunks.reserve(world_width_chunks * world_height_chunks);
        for (int x = 0; x < world_width_chunks; ++x)
        {
            float offset_x = x * chunk_size.width * tile_size.width;
            for (int y = 0; y < world_height_chunks; ++y)
            {
                float offset_y = y * chunk_size.height * tile_size.height;
                std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(
                    device, 
                    texture,
                    chunk_size,
                    tile_size,
                    tile_size_pixels,
                    ChunkOffset(offset_x, offset_y)
                    );
                //chunk->setGrid(grid);
                chunks.emplace_back(chunk);
            }
        }
    }

    std::shared_ptr<Chunk> GpuTilemapSDL::getChunk(int x, int y)
    {
        return chunks[y + x * world_size.height];
    }
}
