#pragma once
#include <cmath>
#include <filesystem>
#include <memory>
#include <format>

#include "GpuSampler.hpp"
#include "Surface.hpp"
#include <graphics/CommandBuffer.hpp>

#include "Texture.hpp"
#include "SDL3/SDL_gpu.h"

namespace graphics
{
	class GpuTextureSDL : public Texture
	{
	public:
		GpuTextureSDL() = default;
		GpuTextureSDL(std::shared_ptr<SDL_GPUDevice> device, const Surface& surface, std::shared_ptr<GpuSampler> sampler);
		~GpuTextureSDL() override;

		// Getters
        int width() const override { return m_surface.get()->w;}
        int height() const override { return m_surface.get()->h;}
        int pitch() const override { return m_surface.get()->pitch;}
        const void* pixels() const override { return m_surface.get()->pixels;}

		std::shared_ptr<GpuSampler> getSampler() const;

		template<typename Self>
		auto&& get(this Self&& self);

	private:
		std::shared_ptr<SDL_GPUDevice> device = nullptr;
		std::shared_ptr<GpuSampler> sampler = nullptr;
		SDL_GPUTexture* texture = nullptr;
		Surface m_surface;
	};

	template <typename Self>
	auto&& GpuTextureSDL::get(this Self&& self)
	{
		return self.texture;
	}

	inline GpuTextureSDL::GpuTextureSDL(std::shared_ptr<SDL_GPUDevice> device, const Surface& surface, std::shared_ptr<GpuSampler> sampler)
		: device{device}
		, sampler{sampler}
		, m_surface{surface}
	{
		SDL_Surface* image_data = surface.get();

		SDL_GPUTextureCreateInfo texture_create_info = {};
		texture_create_info.type = SDL_GPU_TEXTURETYPE_2D;
		texture_create_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
		texture_create_info.width = image_data->w;
		texture_create_info.height = image_data->h;
		texture_create_info.layer_count_or_depth = 1;
		texture_create_info.num_levels = 1;
		texture_create_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
		
		texture = SDL_CreateGPUTexture(device.get(), &texture_create_info);

		if (!texture)
		{
			throw std::runtime_error{ std::format("Could not create GPU texture: {}", SDL_GetError()) };
		}
	}

	inline GpuTextureSDL::~GpuTextureSDL()
	{
		if (texture) SDL_ReleaseGPUTexture(device.get(), texture);
	}
}
