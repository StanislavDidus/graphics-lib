#include <iostream>
#include <graphics/RenderBatch.hpp>
#include <graphics/exception.hpp>
#include "graphics/config.hpp"

#include "../../extern/SDL/src/video/khronos/vulkan/vulkan_core.h"

namespace graphics
{
	void Batch::closeBatch()
	{
		closed = true;
	}

	bool Batch::isClosed() const
	{
		return closed;
	}

	bool ::graphics::SpriteBatch::canBatch(const DrawObject& draw_object) const
	{
		return std::visit([&](auto&& object) -> bool
		{
			using T = std::decay_t<decltype(object)>;
			
			if constexpr (std::is_same_v<T, GpuSprite>)
			{
				return (object.texture == texture && object.sampler == sampler) || sprites.empty();
			}
			else
			{
				return false;
			}
		}, draw_object);
	}

	void SpriteBatch::addToBatch(const DrawObject& draw_object)
	{
		std::visit([&](auto&& object)
		{
			using T = std::decay_t<decltype(object)>;
			
			if constexpr (std::is_same_v<T, GpuSprite>)
			{
				if (sprites.empty())
				{
					sprites.emplace_back(object.data);
					texture = object.texture;
					sampler = object.sampler;
				}
				else
				{
					sprites.emplace_back(object.data);
				}
			}
			else
			{
				throw graphics_error{"Attempted to add a wrong object type."};
			}
		}, draw_object);
	}

	void SpriteBatch::pushAllToGPU(SDL_GPUCopyPass* copy_pass, const PushSpriteData& push_sprite_data, const PushVertexData& push_vertex_data, const
	                               PushLineData& push_line_data)
	{
		if (sprites.empty()) return;

		auto* data = push_sprite_data.transfer_buffer.map<SpriteData>(false);
		data += push_sprite_data.offset;
		SDL_memcpy(data, sprites.data(), sprites.size() * sizeof(SpriteData));
		push_sprite_data.transfer_buffer.unmap();
		
		// Upload vertices
		SDL_GPUTransferBufferLocation vertices_transfer_info = {};
		vertices_transfer_info.transfer_buffer = push_sprite_data.transfer_buffer.get();
		vertices_transfer_info.offset = push_sprite_data.offset * sizeof(SpriteData);
		SDL_GPUBufferRegion vertices_buffer_region = {};
		vertices_buffer_region.buffer = push_sprite_data.buffer.get();
		vertices_buffer_region.offset = push_sprite_data.offset * sizeof(SpriteData);
		vertices_buffer_region.size = sprites.size() * sizeof(SpriteData);

		SDL_UploadToGPUBuffer(copy_pass, &vertices_transfer_info, &vertices_buffer_region, false);
		
		push_sprite_data.offset += sprites.size();
	}

	void SpriteBatch::renderAllOnGPU(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer,
	                                 const glm::mat4& world_matrix, const RenderSpriteData& render_sprite_data, const RenderVertexData& render_vertex_data, const
	                                 RenderLineData& render_line_data, const RenderChunkData& render_chunk_data)
	{
		if (sprites.empty()) return;
		
		SDL_GPUTextureSamplerBinding texture_sampler_binding;
		texture_sampler_binding.texture = texture;
		texture_sampler_binding.sampler = sampler;
		SDL_BindGPUFragmentSamplers(render_pass, 0, &texture_sampler_binding, 1);

		SDL_BindGPUGraphicsPipeline(render_pass, render_sprite_data.graphics_pipeline->get());
		SDL_BindGPUVertexStorageBuffers(render_pass, 0, &render_sprite_data.buffer.get(), 1);
		SDL_PushGPUVertexUniformData(command_buffer.get(), 0, &world_matrix, sizeof(glm::mat4));
		render_sprite_data.sprite_uniform.index = render_sprite_data.offset;
		SDL_PushGPUVertexUniformData(command_buffer.get(), 1, &render_sprite_data.sprite_uniform, sizeof(SpriteUniform));
		SDL_DrawGPUPrimitives(render_pass, 6, sprites.size(), 0, 0);

		render_sprite_data.offset += sprites.size();
	}

	bool VertexBatch::canBatch(const DrawObject& draw_object) const
	{
		return std::visit([&](auto&& object) -> bool
		{
			using T = std::decay_t<decltype(object)>;
			
			if constexpr (std::is_same_v<T, RectangleData>)
			{
				return true;
			}
			else
			{
				return false;
			}
		}, draw_object);
	}

	void VertexBatch::addToBatch(const DrawObject& draw_object)
	{
		std::visit([&](auto&& object)
		{
			using T = std::decay_t<decltype(object)>;
			
			if constexpr (std::is_same_v<T, RectangleData>)
			{
				vertices.reserve(vertices.size() + 6);

				const auto& rectangle_data = object;
				vertices.emplace_back
				(
					rectangle_data.position.x,
					rectangle_data.position.y,
					0.0f,
					rectangle_data.color.r,
					rectangle_data.color.g,
					rectangle_data.color.b,
					rectangle_data.color.a
				);
				vertices.emplace_back
				(
					rectangle_data.position.x,
					rectangle_data.position.y + rectangle_data.size.y,
					0.0f,
					rectangle_data.color.r,
					rectangle_data.color.g,
					rectangle_data.color.b,
					rectangle_data.color.a
				);
				vertices.emplace_back
				(
					rectangle_data.position.x + rectangle_data.size.x,
					rectangle_data.position.y + rectangle_data.size.y,
					0.0f,
					rectangle_data.color.r,
					rectangle_data.color.g,
					rectangle_data.color.b,
					rectangle_data.color.a
				);
				vertices.emplace_back
				(
					rectangle_data.position.x + rectangle_data.size.x,
					rectangle_data.position.y + rectangle_data.size.y,
					0.0f,
					rectangle_data.color.r,
					rectangle_data.color.g,
					rectangle_data.color.b,
					rectangle_data.color.a
				);

				vertices.emplace_back
				(
					rectangle_data.position.x + rectangle_data.size.x,
					rectangle_data.position.y,
					0.0f,
					rectangle_data.color.r,
					rectangle_data.color.g,
					rectangle_data.color.b,
					rectangle_data.color.a
				);
				vertices.emplace_back
				(
					rectangle_data.position.x,
					rectangle_data.position.y,
					0.0f,
					rectangle_data.color.r,
					rectangle_data.color.g,
					rectangle_data.color.b,
					rectangle_data.color.a
				);
			}
			else
			{
				throw graphics_error{"Attempted to add a wrong object type."};
			}
		}, draw_object);
	}

	void VertexBatch::pushAllToGPU(SDL_GPUCopyPass* copy_pass, const PushSpriteData& push_sprite_data, const PushVertexData& push_vertex_data, const
	                               PushLineData& push_line_data)
	{
		if (vertices.empty()) return;

		auto* data = push_vertex_data.transfer_buffer.map<Vertex>(false);
		data += push_vertex_data.offset;
		SDL_memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
		push_vertex_data.transfer_buffer.unmap();
		
		// Upload vertices
		SDL_GPUTransferBufferLocation vertices_transfer_info = {};
		vertices_transfer_info.transfer_buffer = push_vertex_data.transfer_buffer.get();
		vertices_transfer_info.offset = push_vertex_data.offset * sizeof(Vertex);
		SDL_GPUBufferRegion vertices_buffer_region = {};
		vertices_buffer_region.buffer = push_vertex_data.buffer.get();
		vertices_buffer_region.offset = push_vertex_data.offset * sizeof(Vertex);
		vertices_buffer_region.size = vertices.size() * sizeof(Vertex);

		SDL_UploadToGPUBuffer(copy_pass, &vertices_transfer_info, &vertices_buffer_region, false);
		
		push_vertex_data.offset += vertices.size();
	}

	void VertexBatch::renderAllOnGPU(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer,
	                                 const glm::mat4& world_matrix, const RenderSpriteData& render_sprite_data, const RenderVertexData& render_vertex_data, const
	                                 RenderLineData& render_line_data, const RenderChunkData& render_chunk_data)
	{
		SDL_BindGPUGraphicsPipeline(render_pass, render_vertex_data.graphics_pipeline->get());

		SDL_GPUBufferBinding buffer_binding{};
		buffer_binding.offset = render_vertex_data.offset * sizeof(Vertex);
		buffer_binding.buffer = render_vertex_data.buffer.get();
		SDL_BindGPUVertexBuffers(render_pass, 0, &buffer_binding , 1);
		SDL_PushGPUVertexUniformData(command_buffer.get(), 0, &world_matrix, sizeof(glm::mat4));
		SDL_DrawGPUPrimitives(render_pass, vertices.size(), 1, 0, 0);

		render_vertex_data.offset += vertices.size();
	}

	bool LineBatch::canBatch(const DrawObject& draw_object) const
	{
		return std::visit([&](auto&& object) -> bool
		{
			using T = std::decay_t<decltype(object)>;
				
			if constexpr (std::is_same_v<T, LineData>)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}, draw_object);
	}

	void LineBatch::addToBatch(const DrawObject& draw_object)
	{
		std::visit([&](auto&& object)
		{
			using T = std::decay_t<decltype(object)>;
				
			if constexpr (std::is_same_v<T, LineData>)
			{
				vertices.reserve(vertices.size() + 2);
				const auto& line_data = object;
				
				vertices.emplace_back(line_data.x1, line_data.y1, 0.0f, line_data.color.r, line_data.color.g, line_data.color.b, line_data.color.a);
				vertices.emplace_back(line_data.x2, line_data.y2, 0.0f, line_data.color.r, line_data.color.g, line_data.color.b, line_data.color.a);
			}
			else
			{
				throw graphics_error{"Attempted to add a wrong object type."};
			}
		}, draw_object);
	}

	void LineBatch::pushAllToGPU(SDL_GPUCopyPass* copy_pass, const PushSpriteData& push_sprite_data,
		const PushVertexData& push_vertex_data, const PushLineData& push_line_data)
	{
		if (vertices.empty()) return;
	
		Vertex* data = push_line_data.transfer_buffer.map<Vertex>(false);
		data += push_line_data.offset;
		SDL_memcpy(data, vertices.data(), vertices.size() * sizeof(Vertex));
		push_line_data.transfer_buffer.unmap();
		
		// Upload vertices
		SDL_GPUTransferBufferLocation vertices_transfer_info = {};
		vertices_transfer_info.transfer_buffer = push_line_data.transfer_buffer.get();
		vertices_transfer_info.offset = push_line_data.offset * sizeof(Vertex);
		SDL_GPUBufferRegion vertices_buffer_region = {};
		vertices_buffer_region.buffer = push_line_data.buffer.get();
		vertices_buffer_region.offset = push_line_data.offset * sizeof(Vertex);
		vertices_buffer_region.size = vertices.size() * sizeof(Vertex);

		SDL_UploadToGPUBuffer(copy_pass, &vertices_transfer_info, &vertices_buffer_region, false);
		
		push_line_data.offset += vertices.size();
	}

	void LineBatch::renderAllOnGPU(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer,
	                               const glm::mat4& world_matrix, const RenderSpriteData& render_sprite_data,
	                               const RenderVertexData& render_vertex_data, const RenderLineData& render_line_data, const RenderChunkData&
	                               render_chunk_data)
	{
		SDL_BindGPUGraphicsPipeline(render_pass, render_line_data.graphics_pipeline->get());

		SDL_GPUBufferBinding buffer_binding{};
		buffer_binding.offset = render_line_data.offset * sizeof(Vertex);
		buffer_binding.buffer = render_line_data.buffer.get();
		SDL_BindGPUVertexBuffers(render_pass, 0, &buffer_binding , 1);
		SDL_PushGPUVertexUniformData(command_buffer.get(), 0, &world_matrix, sizeof(glm::mat4));
		SDL_DrawGPUPrimitives(render_pass, vertices.size(), 1, 0, 0);
		
		render_line_data.offset += vertices.size();
	}

	bool ChunkBatch::canBatch(const DrawObject& draw_object) const
	{
		return std::visit([&](auto&& object) -> bool
		{
			using T = std::decay_t<decltype(object)>;
				
			if constexpr (std::is_same_v<T, ChunkData>)
			{
				return true;
			}
			else 
			{
				return false;
			}
		}, draw_object);
	}

	void ChunkBatch::addToBatch(const DrawObject& draw_object)
	{
		std::visit([&](auto&& object)
		{
			using T = std::decay_t<decltype(object)>;
				
			if constexpr (std::is_same_v<T, ChunkData>)
			{
				chunks.emplace_back(object);
			}
			else
			{
				throw graphics_error{"Attempted to add a wrong object type."};
			}
		}, draw_object);
	}

	void ChunkBatch::pushAllToGPU(SDL_GPUCopyPass* copy_pass, const PushSpriteData& push_sprite_data,
		const PushVertexData& push_vertex_data, const PushLineData& push_line_data)
	{
		
	}

	void ChunkBatch::renderAllOnGPU(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer,
	                                const glm::mat4& world_matrix, const RenderSpriteData& render_sprite_data,
	                                const RenderVertexData& render_vertex_data, const RenderLineData& render_line_data, const RenderChunkData&
	                                render_chunk_data)
	{
		for (const auto& chunkp : chunks)
		{
			SDL_BindGPUGraphicsPipeline(render_pass, render_chunk_data.graphics_pipeline->get());

			auto& chunk = chunkp.chunk;
			
			SDL_GPUTextureSamplerBinding texture_sampler_binding = {};
			texture_sampler_binding.texture = chunk->getTexture()->get();
			texture_sampler_binding.sampler = chunk->getTexture()->getSampler()->get();
			SDL_BindGPUFragmentSamplers(render_pass, 0, &texture_sampler_binding, 1);

			SDL_GPUBuffer* buffer = chunk->getTileBuffer();
			SDL_BindGPUVertexStorageBuffers(render_pass, 0, &buffer, 1);
			SDL_GPUBuffer* sprite_buffer = chunk->getSpriteBuffer();
			SDL_BindGPUVertexStorageBuffers(render_pass, 1, &sprite_buffer, 1);
			SDL_PushGPUVertexUniformData(command_buffer.get(), 0, &world_matrix, sizeof(glm::mat4));
			SDL_PushGPUVertexUniformData(command_buffer.get(), 1, &chunkp.position, sizeof(glm::vec3));
			SDL_DrawGPUPrimitives(render_pass, 6, chunk->getSize(), 0, 0);
		}
	}

	Batcher::Batcher(const std::shared_ptr<SDL_GPUDevice>& device,
	                 std::shared_ptr<GpuGraphicsPipeline> sprite_graphics_pipeline,
	                 std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline,
	                 std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline,
	                 std::shared_ptr<GpuGraphicsPipeline> chunk_graphics_pipeline)
		: sprite_graphics_pipeline{std::move(sprite_graphics_pipeline)}
		, vertex_graphics_pipeline{std::move(vertex_graphics_pipeline)}
		, line_graphics_pipeline{std::move(line_graphics_pipeline)}
		, chunk_graphics_pipeline{std::move(chunk_graphics_pipeline)}
		, sprite_buffer{device, static_cast<Uint32>(MAX_SPRITES_RENDERED * sizeof(SpriteData)), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ}
		, sprite_transfer_buffer(device, static_cast<Uint32>(MAX_SPRITES_RENDERED * sizeof(SpriteData)), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD)
		, vertex_buffer{device, static_cast<Uint32>(MAX_RECTANGLES_RENDERED * 6 * sizeof(Vertex)), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ}
		, vertex_transfer_buffer(device, static_cast<Uint32>(MAX_RECTANGLES_RENDERED * 6 * sizeof(Vertex)), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD)
		, line_buffer{device, static_cast<Uint32>(MAX_LINES_RENDERED * 2 * sizeof(Vertex)), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ}
		, line_transfer_buffer(device, static_cast<Uint32>(MAX_LINES_RENDERED * 2 * sizeof(Vertex)), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD)
	{
	}

	void Batcher::setWorldMatrix(const glm::mat4& world_matrix)
	{
		this->world_matrix = world_matrix;
	}

	bool Batcher::canBatch(const DrawObject& draw_object) const
	{
		if (batches.empty())		
			return true;
		return batches.back()->canBatch(draw_object);
	}

	void Batcher::closeBatch()
	{
		if (batches.empty())
			return;
		batches.back()->closeBatch();
	}

	void Batcher::addToBatch(const DrawObject& draw_object)
	{
		if (batches.empty())
			createNewBatch(draw_object);
		const auto& last_batch = batches.back();
			
		if (last_batch->isClosed())
			createNewBatch(draw_object);
		const auto& new_last_batch = batches.back();
		
		new_last_batch->addToBatch(draw_object);
	}

	void Batcher::pushAllToGPU(SDL_GPUCopyPass* copy_pass)
	{
		sprite_offset = 0;
		vertex_offset = 0;
		line_offset = 0;
		for (const auto& batch : batches)
		{
			batch->pushAllToGPU(copy_pass,
			                    PushSpriteData{sprite_buffer, sprite_transfer_buffer, sprite_offset},
			                    PushVertexData{vertex_buffer, vertex_transfer_buffer, vertex_offset},
			                    PushLineData{line_buffer, line_transfer_buffer, line_offset});
		}
	}

	void Batcher::renderAll(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer)
	{
		sprite_offset = 0;
		vertex_offset = 0;
		line_offset = 0;
		for (const auto& batch : batches)
		{
			batch->renderAllOnGPU(render_pass, command_buffer, world_matrix,
			                      RenderSpriteData{sprite_graphics_pipeline, sprite_buffer, sprite_uniform, sprite_offset},
			                      RenderVertexData{vertex_graphics_pipeline, vertex_buffer, vertex_offset},
			                      RenderLineData{line_graphics_pipeline, line_buffer, line_offset},
			                      RenderChunkData{chunk_graphics_pipeline});
		}
		batches.clear();
	}

	void Batcher::createNewBatch(const DrawObject& draw_object)
	{
		std::visit([&](auto&& object)
		{
			using T = std::decay_t<decltype(object)>;
		
			if constexpr(std::is_same_v<T, GpuSprite>)
			{
				batches.emplace_back(std::make_unique<SpriteBatch>());
			}
			else if constexpr (std::is_same_v<T, RectangleData>)
			{
				batches.emplace_back(std::make_unique<VertexBatch>());
			}
			else if constexpr(std::is_same_v<T, LineData>)
			{
				batches.emplace_back(std::make_unique<LineBatch>());
			}
			else if constexpr(std::is_same_v<T, ChunkData>)
			{
				batches.emplace_back(std::make_unique<ChunkBatch>());
			}
		}, draw_object);
	}
}
