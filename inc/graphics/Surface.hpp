#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>
#include <graphics/Color.hpp>
#include <graphics/Font.hpp>
#include <optional>

namespace graphics
{
	class Surface
	{
	public:
		Surface() = default;
		Surface(const std::filesystem::path& path);
		Surface(std::shared_ptr<Font> font, const std::string& text, Color color = Color::BLACK, std::optional<int> wrapped_width = std::nullopt);
		~Surface();

		Surface(const Surface &other);
		Surface& operator=(const Surface &other);

		void loadImage(const std::filesystem::path& path);
		void loadText(std::shared_ptr<Font> font, const std::string& text, Color color, std::optional<int> wrapped_length);

		//Getters
		template<typename Self>
		auto&& get(this Self&& self) {return self.surface;}
	private:
		SDL_Surface* surface = nullptr;
	};
} // namespace graphics