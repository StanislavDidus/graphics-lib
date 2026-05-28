#include "Chunk.hpp"

namespace graphics
{
   Chunk::Chunk(ChunkSize chunk_size, const std::vector<int>& grid)
      : grid(grid)
      , chunk_size(chunk_size)
   {
      int size = chunk_size.width * chunk_size.height;
      this->grid.resize(size);
   }
}