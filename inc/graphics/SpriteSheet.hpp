#pragma once

#include <memory>
#include <vector>

#include <graphics/Renderer.hpp>
#include <graphics/Sprite.hpp>
#include "glm/glm.hpp"

namespace graphics
{
	using SpriteList = std::vector<std::pair<std::string, SDL_FRect>>;

	class SpriteSheet
	{
	public:
		SpriteSheet(graphics::Renderer& renderer, const std::filesystem::path& path, const SpriteList& sprite_list, TextureScaleMode scale_mode = TextureScaleMode::LINEAR, TextureAddressMode address_mode = TextureAddressMode::CLAMP);
		SpriteSheet(graphics::Renderer& renderer, const Surface& surface, const SpriteList& sprite_list, TextureScaleMode scale_mode = TextureScaleMode::LINEAR, TextureAddressMode address_mode = TextureAddressMode::CLAMP);

		SpriteSheet(const SpriteSheet& other) = delete;
		SpriteSheet(SpriteSheet&& other) noexcept = delete;
		SpriteSheet& operator=(const SpriteSheet& other) = delete;
		SpriteSheet& operator=(SpriteSheet&& other) noexcept = delete;

		std::shared_ptr<Texture> getTexture() const { return texture; }

		template<typename Self>
		auto&& getSprite(this Self&& self, const std::string& name);
		template<typename Self>
		auto&& getSprite(this Self&& self, size_t index);

		template<typename Self>
		auto&& operator[](this Self&& self, size_t index);

	private:
		std::shared_ptr<Texture> texture = nullptr;

		std::vector<Sprite> sprites;
		std::unordered_map<std::string, size_t> nameToIndex;
	};

	template <typename Self>
	auto&& SpriteSheet::getSprite(this Self&& self, const std::string& name)
	{
		return self.sprites[self.nameToIndex[name]];
	}

	template <typename Self>
	auto&& SpriteSheet::getSprite(this Self&& self, size_t index)
	{
		return self.sprites[index];
	}

	template <typename Self>
	auto&& SpriteSheet::operator[](this Self&& self, size_t index)
	{
		return self.sprites[index];
	}
} // namespace graphics
