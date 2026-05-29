#include <iostream>
#include <graphics/Sprite.hpp>
#include <utility>

namespace graphics
{
	Sprite::Sprite(std::shared_ptr<Texture> texture)
	: texture(texture)
	, rect{ 0.0f, 0.0f, 1.0f, 1.0f}
	{
	}

	Sprite::Sprite(std::shared_ptr<Texture> texture, const SDL_FRect& rect)
	: texture(texture)
	, color{Color::WHITE}
	, rect(
		rect.x / texture->width(),
		rect.y / texture->height(),
		rect.w / texture->width(),
		rect.h / texture->height()
		  )
	{

	}

	const SDL_FRect& Sprite::getRect() const
	{
		return rect;
	}

	std::shared_ptr<Texture> Sprite::getTexture() const
	{
		return texture;
	}

	const Color& Sprite::getColor() const
	{
		return color;
	}

	void Sprite::setColor(const Color& color)
	{	
		this->color = color;
	}
} // namespace graphics