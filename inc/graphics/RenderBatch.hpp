#pragma once
#include <vector>

#include "CommandBuffer.hpp"
#include "Vertex.hpp"
#include "GpuBuffer.hpp"
#include "GpuGraphicsPipeline.hpp"
#include "GpuTransferBuffer.hpp"
#include "graphics/BatchHelpers.hpp"
#include "glm/mat4x4.hpp"

namespace graphics
{
	extern int MAX_SPRITES_RENDERED;
	extern int MAX_RECTANGLES_RENDERED;
	extern int MAX_LINES_RENDERED;
	extern int MAX_TILEMAPS_RENDERED;
	
	class Batch
	{
	public:
		Batch() = default;
		virtual ~Batch() = default;
		
		virtual bool canBatch(const DrawObject& draw_object) const = 0;
		void closeBatch();
		virtual void addToBatch(const DrawObject& draw_object) = 0;
		
		virtual void pushAllToGPU(
			SDL_GPUCopyPass* copy_pass,
			const PushSpriteData& push_sprite_data,
			const PushVertexData& push_vertex_data,
			const PushLineData& push_line_data) = 0;
		
		virtual void renderAllOnGPU(
			SDL_GPURenderPass* render_pass,
			CommandBuffer& command_buffer,
			const glm::mat4& world_matrix,
			const RenderSpriteData& render_sprite_data,
			const RenderVertexData& render_vertex_data,
			const RenderLineData& render_line_data,
			const RenderChunkData& render_chunk_data) = 0;
		
		bool isClosed() const;
	private:
		bool closed = false;	
	};
	
	class SpriteBatch : public Batch
	{
	public:
		SpriteBatch() = default;
		~SpriteBatch() override = default;
		
		bool canBatch(const DrawObject& draw_object) const override;
		void addToBatch(const DrawObject& draw_object) override;
		
		void pushAllToGPU(
			SDL_GPUCopyPass* copy_pass, const PushSpriteData& push_sprite_data, const PushVertexData& push_vertex_data, const
			PushLineData& push_line_data) override;
		
		void renderAllOnGPU(
			SDL_GPURenderPass* render_pass,
			CommandBuffer& command_buffer,
			const glm::mat4& world_matrix,
			const RenderSpriteData& render_sprite_data,
			const RenderVertexData& render_vertex_data,
			const RenderLineData& render_line_data,
			const RenderChunkData& render_chunk_data) override;	
	private:
		std::shared_ptr<GpuTexture> texture;	
		
		std::vector<SpriteData> sprites;
	};
	
	class VertexBatch : public Batch
	{
	public:
		VertexBatch() = default;
		~VertexBatch() override = default;
		
		bool canBatch(const DrawObject& draw_object) const override;
		void addToBatch(const DrawObject& draw_object) override;
		
		void pushAllToGPU(
			SDL_GPUCopyPass* copy_pass, const PushSpriteData& push_sprite_data, const PushVertexData& push_vertex_data, const
			PushLineData& push_line_data) override;
		
		void renderAllOnGPU(
			SDL_GPURenderPass* render_pass,
			CommandBuffer& command_buffer,
			const glm::mat4& world_matrix,
			const RenderSpriteData& render_sprite_data,
			const RenderVertexData& render_vertex_data,
			const RenderLineData& render_line_data,
			const RenderChunkData& render_chunk_data) override;
	private:
		std::vector<Vertex> vertices;
	};
	
	class LineBatch : public Batch
	{
	public:
		LineBatch() = default;
		~LineBatch() override = default;
		
		bool canBatch(const DrawObject& draw_object) const override;
		void addToBatch(const DrawObject& draw_object) override;
		
		void pushAllToGPU(
			SDL_GPUCopyPass* copy_pass,
			const PushSpriteData& push_sprite_data,
			const PushVertexData& push_vertex_data,
			const PushLineData& push_line_data) override;
		
		void renderAllOnGPU(
			SDL_GPURenderPass* render_pass,
			CommandBuffer& command_buffer,
			const glm::mat4& world_matrix,
			const RenderSpriteData& render_sprite_data,
			const RenderVertexData& render_vertex_data,
			const RenderLineData& render_line_data,
			const RenderChunkData& render_chunk_data) override;
	private:
		std::vector<Vertex> vertices;
	};
	
	class ChunkBatch : public Batch
	{
	public:
		ChunkBatch() = default;
		~ChunkBatch() override = default;
		
		bool canBatch(const DrawObject& draw_object) const override;
		void addToBatch(const DrawObject& draw_object) override;
		
		void pushAllToGPU(
			SDL_GPUCopyPass* copy_pass,
			const PushSpriteData& push_sprite_data,
			const PushVertexData& push_vertex_data,
			const PushLineData& push_line_data) override;
		
		void renderAllOnGPU(
			SDL_GPURenderPass* render_pass,
			CommandBuffer& command_buffer,
			const glm::mat4& world_matrix,
			const RenderSpriteData& render_sprite_data,
			const RenderVertexData& render_vertex_data,
			const RenderLineData& render_line_data,
			const RenderChunkData& render_chunk_data) override;
	private:
		std::vector<ChunkData> chunks;
	};
	
	class Batcher
	{
	public:
		Batcher() = default;
		Batcher(
			const std::shared_ptr<SDL_GPUDevice>& device,
			std::shared_ptr<GpuGraphicsPipeline> sprite_graphics_pipeline,
			std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline,
			std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline,
			std::shared_ptr<GpuGraphicsPipeline> chunk_graphics_pipeline
		);
		~Batcher() = default;
		
		void setWorldMatrix(const glm::mat4& world_matrix);
		
		bool canBatch(const DrawObject& draw_object) const;
		void closeBatch();
		void addToBatch(const DrawObject& draw_object);
		
		void pushAllToGPU(SDL_GPUCopyPass* copy_pass);
		void renderAll(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer);
	private:
		void createNewBatch(const DrawObject& draw_object);
		
		std::vector<std::unique_ptr<Batch>> batches;
		
		std::shared_ptr<SDL_GPUDevice> device;
		
		// Graphics pipelines for each object type
		std::shared_ptr<GpuGraphicsPipeline> sprite_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> chunk_graphics_pipeline;
		
		// GPU Buffers 
		GpuBuffer sprite_buffer;
		GpuTransferBuffer sprite_transfer_buffer;
		GpuBuffer vertex_buffer;
		GpuTransferBuffer vertex_transfer_buffer;
		GpuBuffer line_buffer;
		GpuTransferBuffer line_transfer_buffer;
		
		// World matrix that will be applied to all objects
		glm::mat4 world_matrix;
		
		// Additional sprite info
		SpriteUniform sprite_uniform{};
		size_t sprite_offset = 0;
		size_t vertex_offset = 0;
		size_t line_offset = 0;
	};
}
