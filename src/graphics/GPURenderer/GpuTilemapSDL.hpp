#pragma once
#include "graphics/TileMap.hpp"
#include "graphics/GPURenderer/Chunk.hpp"

namespace graphics
{
class GpuTilemapSDL : public graphics::TileMap
{
public:
   GpuTilemapSDL() = default;
    
    GpuTilemapSDL(
          std::shared_ptr<SDL_GPUDevice> device,
          std::shared_ptr<Texture> texture,
          WorldSize world_size,
          TileSize tile_size,
          TileSizePixels tile_size_pixels);
    GpuTilemapSDL(
         std::shared_ptr<SDL_GPUDevice> device,
         std::shared_ptr<Texture> texture,
         WorldSize world_size,
         TileSize tile_size,
         TileSizePixels tile_size_pixels,
         ChunkSize chunk_size);
   ~GpuTilemapSDL() override = default;
    
    void update() override;
    
    // Setters
    void setGrid(int x, int y, const std::vector<int>& grid) override;
    void setTile(int x, int y, int id) override;
    
    // Getters
   const std::vector<std::shared_ptr<Chunk>>& getChunks() const { return chunks; }
private:
    void create();
    std::shared_ptr<Chunk> getChunk(int x, int y);
    
    std::shared_ptr<SDL_GPUDevice> device;
    std::vector<std::shared_ptr<Chunk>> chunks; 
};
}