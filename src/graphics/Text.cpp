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
		if (!m_text)
		{
			throw graphics_error{std::format("Failed to create text: {}", SDL_GetError())};
		}
		
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

	Color Text::getColor() const
	{
		Color color;
		TTF_GetTextColor(m_text, &color.r, &color.g, &color.b, &color.a);
		return color;
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
		if (!TTF_SetTextFont(m_text, font->get()))
		{
			throw graphics_error{std::format("Failed to set font: {}", SDL_GetError())};
		}
	}


	void Text::setColor(const Color& color)
	{
		if (!TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a))
		{
			throw graphics_error{std::format("Failed to set color: {}", SDL_GetError())};
		}
	}

	void Text::setText(const std::string& text)
	{
		if (!TTF_SetTextString(m_text, text.c_str(), text.length()))
		{
			throw graphics_error{std::format("Failed to set text: {}", SDL_GetError())};
		}
	}

	void Text::setWrappedWidth(int wrapped_width)
	{
		if (!TTF_SetTextWrapWidth(m_text, wrapped_width))
		{
			throw graphics_error{std::format("Failed to set wrapped width: {}", SDL_GetError())};
		}
	}
} // namespace graphics