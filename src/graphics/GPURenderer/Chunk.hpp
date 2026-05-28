#pragma once
#include <vector>

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "graphics/GpuBuffer.hpp"
#include "graphics/GpuTextureSDL.hpp"
#include "graphics/Texture.hpp"
#include "graphics/TileMapData.hpp"

namespace graphics
{
   class Chunk
    {
    public:
       struct TileData
       {
           glm::vec4 position;
           glm::vec2 size; 
           glm::vec2 uv;
           glm::vec4 color;
       };
       
        Chunk() = default;
        Chunk(
            std::shared_ptr<SDL_GPUDevice> device,
            std::shared_ptr<Texture> texture,
            ChunkSize chunk_size,
            TileSize tile_size,
            TileSizePixels tile_size_pixels,
            ChunkOffset chunk_offset
            );
        ~Chunk() = default;
       
       void update();
       
       // Setters
       void setGrid(const std::vector<int>& grid);
       void setTile(int x, int y, int id);
       
       // Getters
       std::shared_ptr<GpuTextureSDL> getTexture() { return std::static_pointer_cast<GpuTextureSDL>(texture); }
       SDL_GPUBuffer* getTileBuffer() const { return tile_buffer.get(); }
       SDL_GPUBuffer* getSpriteBuffer() const { return sprite_buffer.get(); }
       int getSize() const { return size; }
    private:
       GpuBuffer tile_buffer;
       GpuBuffer sprite_buffer;
       std::shared_ptr<SDL_GPUDevice> device;
       std::shared_ptr<Texture> texture;
       std::vector<int> grid;
       
       ChunkSize chunk_size;
       int size = 1;
       bool is_dirty = false;
    };
}
