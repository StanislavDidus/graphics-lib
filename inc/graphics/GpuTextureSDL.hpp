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
		auto&& get(this Self&& self){ return self.texture; }

	private:
		std::shared_ptr<SDL_GPUDevice> device = nullptr;
		std::shared_ptr<GpuSampler> sampler = nullptr;
		SDL_GPUTexture* texture = nullptr;
		Surface m_surface;
	};
}
