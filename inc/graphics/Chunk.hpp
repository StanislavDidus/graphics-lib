#pragma once

#include "GpuBuffer.hpp"
#include "GpuTextureSDL.hpp"
#include <glm/vec4.hpp>

namespace graphics
{
	/*struct TileData
	{
		glm::vec4 position; // Position3 Padding1
		glm::vec4 size_uv; // Size2 UV2
		glm::vec4 color; // Color4
	};*/

	class Chunk
	{
	public:
		Chunk() = default;
		Chunk(const std::vector<int>& grid);
		~Chunk() = default;
		
		// Setters
		void setGrid(const std::vector<int>& grid) { m_grid = grid; }
		
		// Getters
		const std::vector<int>& getGrid() const { return m_grid; }
	private:
		std::vector<int> m_grid;
	};
}
