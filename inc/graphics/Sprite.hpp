#pragma once

#include <memory>

#include "Color.hpp"
#include "GpuTextureSDL.hpp"
#include "SDL3/SDL_render.h"

namespace graphics
{
	class Sprite
	{
	public:
		Sprite() = default;
		Sprite(std::shared_ptr<Texture> texture);
		Sprite(std::shared_ptr<Texture> texture, const SDL_FRect& rect);

		//Getters
		const SDL_FRect& getRect() const;
		std::shared_ptr<Texture> getTexture() const;
		const Color& getColor() const;

		//Setters
		void setColor(const Color& color);

	private:
		std::shared_ptr<Texture> texture = nullptr;
		SDL_FRect rect;

		Color color = Color::WHITE;
	};

} // namespace graphics