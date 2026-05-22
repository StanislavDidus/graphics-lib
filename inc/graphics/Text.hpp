#pragma once

#include <string>
#include "Font.hpp"
#include "Color.hpp"
#include "GpuTextureSDL.hpp"
#include "glm/vec2.hpp"

struct SDL_Texture;

namespace graphics
{
	class TextEngine;
	class Renderer;
	class Surface;

	class  Text
	{
	public:
		Text(TextEngine& text_engine, std::shared_ptr<Font> font, const std::string& text, Color color = Color::WHITE);
		~Text() = default;

		Text(const Text& other) = delete;
		Text(Text&& other) noexcept = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) noexcept = delete;

		//Getters
		std::shared_ptr<Texture> getTexture() const;
		const std::string& getText() const;
		glm::vec2 getTextSize(const glm::vec2& scale) const;

		//Setters
		void setFont(std::shared_ptr<Font> font);
		void setColor(Color color);
		void setText(const std::string& text);
		void setWrappedWidth(int wrapped_width);

		void updateText(graphics::Renderer& renderer);
	private:
		void generateTextTexture(graphics::Renderer& renderer);

		std::string text;
		std::shared_ptr<Texture> texture = nullptr;
		std::shared_ptr<Font> font;
		Color color;
		std::optional<int> wrapped_width = std::nullopt;

		bool is_dirty = false;

		TTF_Text* m_text = nullptr;
	};

} // namespace graphics