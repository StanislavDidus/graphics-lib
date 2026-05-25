#include <iostream>

#include <graphics/Text.hpp>

#include <graphics/Color.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/Surface.hpp>

#include "graphics/exception.hpp"
#include "graphics/TextEngine.hpp"

namespace graphics
{
	Text::Text(const TextEngine& text_engine, std::shared_ptr<Font> font, const std::string& text, Color color)
		: text{text}
	{
		m_text = TTF_CreateText(text_engine.get(), font->get(), text.data(), text.length());
		setColor(color);
	}

	Text::~Text()
	{
		if (m_text)
			TTF_DestroyText(m_text);
	}

	std::shared_ptr<Texture> Text::getTexture() const
	{
		return nullptr;
	}

	const std::string& Text::getText() const
	{
		return text;
	}

	glm::vec2 Text::getSize() const
	{
		int w = 0;
		int h = 0;
		TTF_GetTextSize(m_text, &w, &h);
		return {static_cast<float>(w), static_cast<float>(h)};
	}

	TTF_GPUAtlasDrawSequence* Text::getGPUDrawData() const
	{
		TTF_GPUAtlasDrawSequence* result = TTF_GetGPUTextDrawData(m_text);
		if (!result)
		{
			throw graphics_error{std::format("Error getting GPUDrawData: {}", SDL_GetError())};
		}
		return result;
	}

	TTF_Text* Text::getRendererDrawData() const
	{
		return m_text;
	}

	void Text::setFont(const std::shared_ptr<Font>& font)
	{
		TTF_SetTextFont(m_text, font->get());
	}


	void Text::setColor(const Color& color)
	{
		TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
	}

	void Text::setText(const std::string& text)
	{
		TTF_SetTextString(m_text, text.c_str(), text.length());
	}

	void Text::setWrappedWidth(int wrapped_width)
	{
		TTF_SetTextWrapWidth(m_text, wrapped_width);
	}
} // namespace graphics