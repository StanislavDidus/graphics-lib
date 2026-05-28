#pragma once


#include <iostream>
#include <unordered_map>

#include "SDL3/SDL_gpu.h"

#include "Window.hpp"
#include "GpuShader.hpp"
#include "GpuGraphicsPipeline.hpp"
#include "GpuBuffer.hpp"
#include "GpuTextureSDL.hpp"
#include "GpuTransferBuffer.hpp"
#include "Text.hpp"
#include "GpuSampler.hpp"
#include "Sprite.hpp"
#include "TileMap.hpp"
#include "Enums.hpp"

#include "Vertex.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "RenderBatch.hpp"
#include "glm/gtx/transform.hpp"

namespace graphics
{
	class TileMap;

	struct WindowClaimer
	{
		SDL_GPUDevice* device;
		SDL_Window* window;

		WindowClaimer() = default;
		WindowClaimer(SDL_GPUDevice* device, SDL_Window* window) : device{device}, window{window} {}
		~WindowClaimer()
		{
			if (device && window)
				SDL_ReleaseWindowFromGPUDevice(device, window);
		}
	};
	
	static ScreenSize screen_size_uniform;

	class GpuRenderer
	{
	public:
		explicit GpuRenderer(Window& window);
		~GpuRenderer() = default;

		void render();

		std::shared_ptr<GpuTextureSDL> loadTexture(const Surface& surface, const std::string& sampler_name);
		void createNewSampler(const std::string& name, SDL_GPUFilter filter, SDL_GPUSamplerMipmapMode mipmap, SDL_GPUSamplerAddressMode address_mode, std::optional<int> anisotropy = std::nullopt);

		//Getters
		glm::vec2 getView() const;
		float getZoom() const;
		float getAngle() const;
		glm::ivec2 getStandardWindowSize() const;
		const glm::mat4& getWorldMatrix() const;
		glm::vec2 getRenderResolution() const;

		//Setters
		void setView(glm::vec2 view);
		void setZoom(float zoom);
		void setAngle(float angle);
		void setRenderMode(RenderMode render_mode);

		template<typename Self>
		auto&& getDevice(this Self&& self);

		void renderRectangle(float x, float y, float w, float h, RenderType render_type, const Color& color);
		void renderSprite(const Sprite& sprite, float x, float y, float w, float h, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = graphics::Color::WHITE);
		void renderText(const Text& text, float x, float y, float w, float h);
		void renderTexture(std::shared_ptr<GpuTextureSDL> texture, const std::optional<SDL_FRect>& source, const std::optional<SDL_FRect>& destination, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const  Color& color = graphics::Color::WHITE, float
		                   z = 0.0f);
		void renderTileMap(std::shared_ptr<TileMap> tilemap, float x, float y);
	private:
		void initSamplers();
		
		SDL_FRect getCameraRect() const;

		Window& window;
		std::shared_ptr<SDL_GPUDevice> device = nullptr;
		
		RenderMode render_mode = RenderMode::WORLD;

		std::vector<DrawObject> draw_buffer;
		std::vector<DrawObject> ui_draw_buffer;

		// <Render Parameters> //
		glm::vec2 view = {0.0f, 0.0f};
		float zoom = 1.0f;
		float angle = 0.0f; ///< Degrees.
		glm::mat4 world_matrix;
		Uint32 render_resolution_width;
		Uint32 render_resolution_height;
		
		std::unique_ptr<Batcher> batcher;
		std::unique_ptr<Batcher> ui_batcher;

		std::unique_ptr<WindowClaimer> window_claimer;
		std::shared_ptr<GpuGraphicsPipeline> tilemap_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline;
		std::shared_ptr<GpuGraphicsPipeline> texture_graphics_pipeline;

		std::unique_ptr<GpuShader> tilemap_vertex_shader;
		std::unique_ptr<GpuShader> vertex_shader;
		std::unique_ptr<GpuShader> fragment_shader;
		std::unique_ptr<GpuShader> texture_vertex_shader;
		std::unique_ptr<GpuShader> texture_fragment_shader;

		std::unordered_map<std::string, std::shared_ptr<GpuSampler>> samplers;
	};

	template <typename Self>
	auto&& graphics::GpuRenderer::getDevice(this Self&& self)
	{
		return self.device;
	}
}
