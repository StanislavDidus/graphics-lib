#pragma once
#include <vector>

#include "graphics/TileMapData.hpp"

namespace graphics
{
    class Chunk
    {
    public:
        Chunk() = default;
        Chunk(ChunkSize chunk_size, const std::vector<int>& grid);
        ~Chunk() = default;
        
        void update() { is_dirty = false; }
        
        // Setters
        void setGrid(const std::vector<int>& grid) { this->grid = grid; is_dirty = true; }
        void setTile(int x, int y, int id) { grid[x + y * chunk_size.width] = id; is_dirty = true; }
        
        // Getters
        const std::vector<int>& getGrid() const { return grid; }
    private:
        ChunkSize chunk_size;
        std::vector<int> grid;
        bool is_dirty = false;
    };
}
