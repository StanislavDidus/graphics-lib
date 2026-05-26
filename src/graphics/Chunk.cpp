#include <graphics/Chunk.hpp>

#include <graphics/CommandBuffer.hpp>
#include <graphics/GpuTransferBuffer.hpp>

namespace graphics
{
    Chunk::Chunk(const std::vector<int>& grid)
        : m_grid(grid)
    {
    }
}
