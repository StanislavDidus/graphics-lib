#include "Chunk.hpp"
#include "Chunk.hpp"

#include <iostream>

#include "graphics/CommandBuffer.hpp"
#include "graphics/GpuTransferBuffer.hpp"

namespace  graphics
{
    Chunk::Chunk(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<Texture> texture, ChunkSize chunk_size, TileSize tile_size,
        TileSizePixels tile_size_pixels, ChunkOffset chunk_offset)
    : size(chunk_size.width * chunk_size.height)
    , device(device)
    , texture(texture)
    , tile_buffer(device, static_cast<Uint32>(chunk_size.width * chunk_size.height * sizeof(Chunk::TileData)), SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ)
    , sprite_buffer(device, static_cast<Uint32>(chunk_size.width * chunk_size.height * sizeof(int)), SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ)
    , chunk_size(chunk_size)
    {
        GpuTransferBuffer transfer_buffer{ device, static_cast<Uint32>(size * sizeof(TileData)), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD };
	
        TileData* data = transfer_buffer.map<TileData>(false);
        
        int texture_width = texture->width();
        int texture_height = texture->height();
        float tile_width_normalized = static_cast<float>(tile_size_pixels.width) / texture_width;
        float tile_height_normalized = static_cast<float>(tile_size_pixels.height) / texture_height;
        
        for (int y = 0; y < chunk_size.height; ++y)
        {
            for (int x = 0; x < chunk_size.width; ++x)
            {
                int index = x + y * chunk_size.width;
                data[index] = TileData
                {   
                    .position = {chunk_offset.x + x * tile_size.width, chunk_offset.y + y * tile_size.height, 0.0f, 1.0f},
                    .size = {static_cast<float>(tile_size.width), static_cast<float>(tile_size.height)},
                    .uv = { tile_width_normalized, tile_height_normalized},
                    .color = {255.0f, 255.0f, 255.0f, 255.0f}
                };
            }
        }

        transfer_buffer.unmap();

        CommandBuffer command_buffer{device};

        SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(command_buffer.get());

        // Where is
        SDL_GPUTransferBufferLocation location;
        location.transfer_buffer = transfer_buffer.get();
        location.offset = 0;

        // Where to
        SDL_GPUBufferRegion region{};
        region.buffer = tile_buffer.get();
        region.size = size * sizeof(TileData);
        region.offset = 0;

        // Upload
        SDL_UploadToGPUBuffer(copy_pass, &location, &region, true);

        SDL_EndGPUCopyPass(copy_pass);
        
        // Create grid
        grid.resize(size);
    }

    void Chunk::update()
    {
        if (!is_dirty) return;
        
        GpuTransferBuffer transfer_buffer{ device, static_cast<Uint32>(grid.size() * sizeof(int)), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD };
        int* data = transfer_buffer.map<int>(false);
        SDL_memcpy(data, grid.data(), grid.size() * sizeof(int));
        transfer_buffer.unmap();

        CommandBuffer command_buffer{device};

        SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(command_buffer.get());

        // Where is
        SDL_GPUTransferBufferLocation location;
        location.transfer_buffer = transfer_buffer.get();
        location.offset = 0;

        // Where to
        SDL_GPUBufferRegion region{};
        region.buffer = sprite_buffer.get();
        region.size = grid.size() * sizeof(int);
        region.offset = 0;

        // Upload
        SDL_UploadToGPUBuffer(copy_pass, &location, &region, true);

        SDL_EndGPUCopyPass(copy_pass); 
        
        is_dirty = false;
    }

    void Chunk::setGrid(const std::vector<int>& grid)
    {
        assert(grid.size() > 0 && grid.size() <= size);
        
        this->grid = grid;
        
        is_dirty = true; 
    }

    void Chunk::setTile(int x, int y, int id)
    {
        assert(x >= 0 && x < chunk_size.width);
        assert(y >= 0 && y < chunk_size.height);
        
        grid[x + y * chunk_size.width] = id;
        
        is_dirty = true;
    }
}
