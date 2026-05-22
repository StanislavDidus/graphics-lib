#include <iostream>
#include <graphics/Sprite.hpp>
#include <utility>

namespace graphics
{
	Sprite::Sprite(std::shared_ptr<Texture> texture)
	: texture(texture)
	, rect{SDL_FRect{0.0f, 0.0f, static_cast<float>(texture->width()), static_cast<float>(texture->height())}}
	{
		std::cout << texture->width() << std::endl;
		std::cout << texture->height() << std::endl;
	}

	Sprite::Sprite(std::shared_ptr<Texture> texture, const SDL_FRect& rect) : texture(std::move(texture)), rect(rect), color{Color::WHITE}
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