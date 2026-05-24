#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <filesystem>

namespace graphics
{
	class Font
	{
	public:
		Font(const std::filesystem::path& path, int size);
		~Font();

		bool loadFont(const std::filesystem::path& path, int size);

		template<typename Self>
		auto&& get(this Self&& self) { return self.ttf_font; }
	private:
		TTF_Font* ttf_font;
	};

} // namespace graphics