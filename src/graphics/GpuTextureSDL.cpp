//
// Created by stasd on 5/20/26.
//

#include "graphics/GpuTextureSDL.hpp"

namespace graphics
{

	GpuTextureSDL::GpuTextureSDL(std::shared_ptr<SDL_GPUDevice> device, const Surface& surface, std::shared_ptr<GpuSampler> sampler)
		: device{device}
		, sampler{sampler}
		, m_surface{surface}
	{
		SDL_Surface* image_data = m_surface.get();

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

    GpuTextureSDL::~GpuTextureSDL()
    {
        if (texture)
            SDL_ReleaseGPUTexture(device.get(), texture);
    }

    std::shared_ptr<GpuSampler> GpuTextureSDL::getSampler() const
    {
        return sampler;
    }
}
