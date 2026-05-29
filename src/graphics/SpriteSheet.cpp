#include <graphics/SpriteSheet.hpp>

#include <iostream>

#include <graphics/Surface.hpp>

namespace graphics
{
	SpriteSheet::SpriteSheet(graphics::Renderer& renderer, const std::filesystem::path& path,
		const SpriteList& sprite_list, TextureScaleMode scale_mode, TextureAddressMode address_mode)
	{
		texture = renderer.loadTexture(Surface{path}, scale_mode, address_mode);
		
		for (const auto& sprite : sprite_list)
		{
			sprites.emplace_back(texture, sprite.second);
			nameToIndex[sprite.first] = sprites.size() - 1;
		}
	}

	SpriteSheet::SpriteSheet(graphics::Renderer& renderer, const Surface& surface, const SpriteList& sprite_list,
		TextureScaleMode scale_mode, TextureAddressMode address_mode)
	{
		texture = renderer.loadTexture(surface, scale_mode, address_mode);
		
		for (const auto& sprite : sprite_list)
		{
			sprites.emplace_back(texture, sprite.second);
			nameToIndex[sprite.first] = sprites.size() - 1;
		}
	}
} // namespace graphics