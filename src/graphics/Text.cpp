#include <iostream>

#include <graphics/Text.hpp>

#include <graphics/Color.hpp>
#include <graphics/Renderer.hpp>
#include <graphics/Surface.hpp>

#include "graphics/TextEngine.hpp"

namespace graphics
{
	Text::Text(TextEngine& text_engine, std::shared_ptr<Font> font, const std::string& text, Color color)
	{
		m_text = TTF_CreateText(text_engine.get(), font->getFont(), text.c_str(), text.length());
		TTF_SetTextColor(m_text, color.r, color.g, color.b, color.a);
	}

	std::shared_ptr<Texture> Text::getTexture() const
	{
		return nullptr;
	}

	const std::string& Text::getText() const
	{
		return text;
	}

	glm::vec2 Text::getTextSize(const glm::vec2& scale) const
	{
		return {texture->width() * scale.x, texture->height() * scale.y};
	}

	void Text::setFont(std::shared_ptr<Font> font)
	{
		this->font = font;
		is_dirty = true;
	}


	void Text::setColor(Color color)
	{
		this->color = color;
		is_dirty = true;
	}

	void Text::setText(const std::string& text)
	{
		this->text = text;
		is_dirty = true;
	}

	void Text::setWrappedWidth(int wrapped_width)
	{
		this->wrapped_width = wrapped_width;
		is_dirty = true;
	}

	void Text::updateText(graphics::Renderer& renderer)
	{
		if (is_dirty)
		{
			//generateTextTexture(renderer);
			is_dirty = false;
		}
	}

	void Text::generateTextTexture(graphics::Renderer& renderer)
	{
		Surface surface{ font, text, color, wrapped_width };

		//texture = renderer.loadTexture(surface, TextureScaleMode::LINEAR);
	}
} // namespace graphics