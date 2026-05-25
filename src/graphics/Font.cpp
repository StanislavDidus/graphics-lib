#include <graphics/Font.hpp>
#include <iostream>

namespace graphics
{

	Font::Font(const std::filesystem::path& path, int size)
	{
		load(path, size);
	}

	Font::~Font()
	{
		close();
	}

	bool Font::load(const std::filesystem::path& path, int size)
	{
		ttf_font = TTF_OpenFont(path.string().c_str(), size);

		if (!ttf_font)
		{
			std::cerr << "ERROR: Something went wrong: " << SDL_GetError() << std::endl;
			return false;
		}
		return true;
	}

	void Font::close()
	{
		if (ttf_font)
		{
			TTF_CloseFont(ttf_font);
			ttf_font = nullptr;
		}
	}

	void Font::setSize(float size)
	{
		TTF_SetFontSize(ttf_font, size);
	}
} // namespace graphics