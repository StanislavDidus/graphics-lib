#pragma once
#include <vector>

#include "CommandBuffer.hpp"
#include "Vertex.hpp"
#include "GpuBuffer.hpp"
#include "GpuGraphicsPipeline.hpp"
#include "GpuTransferBuffer.hpp"
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
		
		virtual void pushAllToGPU(SDL_GPUCopyPass* copy_pass, GpuBuffer& sprite_buffer, GpuTransferBuffer& sprite_transfer_buffer, size_t& sprite_offset) = 0;
		virtual void renderAllOnGPU(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer, const glm::mat4& world_matrix, std::shared_ptr<
		                            GpuGraphicsPipeline> sprite_graphics_pipeline, GpuBuffer& sprite_buffer, SpriteUniform
		                            & sprite_uniform, size_t& sprite_offset) = 0;
		
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
		
		void pushAllToGPU(SDL_GPUCopyPass* copy_pass, GpuBuffer& sprite_buffer, GpuTransferBuffer& sprite_transfer_buffer, size_t& sprite_offset) override;
		void renderAllOnGPU(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer, const glm::mat4& world_matrix, std::shared_ptr<
		                    GpuGraphicsPipeline> sprite_graphics_pipeline, GpuBuffer& sprite_buffer, SpriteUniform
		                    & sprite_uniform, size_t& sprite_offset) override;	
	private:
		std::shared_ptr<GpuTexture> texture;	
		
		std::vector<SpriteData> sprites;
	};
	
	class Batcher
	{
	public:
		Batcher() = default;
		Batcher(
			const std::shared_ptr<SDL_GPUDevice>& device,
			std::shared_ptr<GpuGraphicsPipeline> sprite_graphics_pipeline,
			std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline,
			std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline
		);
		~Batcher() = default;
		
		void setWorldMatrix(const glm::mat4& world_matrix);
		
		bool canBatch(const DrawObject& draw_object) const;
		void closeBatch();
		void addToBatch(const DrawObject& draw_object);
		
		void pushAllToGPU(SDL_GPUCopyPass* copy_pass);
		void renderAll(SDL_GPURenderPass* render_pass, CommandBuffer& command_buffer);
		void clearAll();
	private:
		void createNewBatch(const DrawObject& draw_object);
		
		std::vector<std::unique_ptr<Batch>> batches;
		
		std::shared_ptr<SDL_GPUDevice> device;
		
		// Graphics pipelines for each object type
		std::shared_ptr<GpuGraphicsPipeline> sprite_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline;
		
		// GPU Buffers 
		GpuBuffer sprite_buffer;
		GpuTransferBuffer sprite_transfer_buffer;
		
		// World matrix that will be applied to all objects
		glm::mat4 world_matrix;
		
		// Additional sprite info
		SpriteUniform sprite_uniform;
		size_t sprite_offset = 0;
	};
	
	/*
	class Batch
	{
	public:
		Batch() = default;
		Batch(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline);
		virtual ~Batch() = default;

		void setMatrix(const glm::mat4& matrix);
		virtual void flushBatch(CommandBuffer& command_buffer, SDL_GPUColorTargetInfo& target_info) = 0;
		virtual void reset() = 0;
	protected:
		std::shared_ptr<SDL_GPUDevice> device;
		std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline;

		size_t offset = 0;
		glm::mat4 matrix;
	};
	class SpriteBatch : public Batch
	{
	public:
		SpriteBatch() = default;
		SpriteBatch(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline);
		~SpriteBatch() override = default;


		void addToBatch(const GpuSprite& sprite);
		void flushBatch(CommandBuffer& command_buffer, SDL_GPUColorTargetInfo& target_info) override;
		bool canBatch(const GpuSprite& gpu_sprite) const;

		void reset() override;
	private:
		std::vector<SpriteData> sprites;
		std::shared_ptr<GpuTexture> texture;

		GpuBuffer storage_buffer;
		GpuTransferBuffer transfer_buffer;

		SpriteUniform sprite_uniform{};
	};
	
	class RectangleBatch : public Batch
	{
	public:
		RectangleBatch() = default;
		RectangleBatch(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline);
		~RectangleBatch() override = default;

		void addToBatch(const RectangleData& rectangle_data);
		void flushBatch(CommandBuffer& command_buffer, SDL_GPUColorTargetInfo& target_info) override;
		bool canBatch(const RectangleData& rectangle_data) const;

		void reset() override;
	private:
		std::vector<Vertex> vertices;

		GpuBuffer vertices_buffer;
		GpuTransferBuffer transfer_buffer;
	};

	class LineBatch : public Batch
	{
	public:
		LineBatch() = default;
		LineBatch(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline);
		~LineBatch() override = default;

		void addToBatch(const LineData& line_data);
		void flushBatch(CommandBuffer& command_buffer, SDL_GPUColorTargetInfo& target_info) override;
		bool canBatch(const LineData& line_data) const;

		void reset() override;
	private:
		std::vector<Vertex> vertices;

		GpuBuffer line_buffer;
		GpuTransferBuffer transfer_buffer;
	};

	class TileMapBatch
	{
	public:
		TileMapBatch() = default;
		TileMapBatch(std::shared_ptr<SDL_GPUDevice> device, std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline);
		~TileMapBatch() = default;

		void setMatrix(const glm::mat4& matrix);

		void addToBatch(const RectangleData& rectangle_data);
		void flushBatch(CommandBuffer& command_buffer, SDL_GPUColorTargetInfo& target_info, bool& first_render);
		bool canBatch(const RectangleData& rectangle_data) const;

		void reset();

	private:
		std::vector<TileMapData> vertices;
		std::shared_ptr<SDL_GPUDevice> device;
		std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline;

		GpuBuffer vertices_buffer;
		GpuTransferBuffer transfer_buffer;

		size_t offset;
		bool first_draw = true;

		glm::mat4 world_matrix;
	};
	*/

}
