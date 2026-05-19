#include <graphics/SpriteSheet.hpp>

#include <iostream>

#include <graphics/Surface.hpp>

namespace graphics
{

	SpriteSheet::SpriteSheet(graphics::GpuRenderer& renderer, const std::filesystem::path& path, const SpriteList& sprite_list, SDL_ScaleMode scale_mode)
	{
		texture = renderer.loadTexture(Surface{path}, "PointClamp");

		for (const auto& sprite : sprite_list)
		{
			sprites.emplace_back(texture, sprite.second);
			nameToIndex[sprite.first] = sprites.size() - 1;
		}
	}

	std::shared_ptr<GpuTextureSDL> SpriteSheet::getTexture() const
	{
		return texture;
	}
} // namespace graphics