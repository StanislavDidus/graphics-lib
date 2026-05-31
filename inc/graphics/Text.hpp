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
		Text(const TextEngine& text_engine, std::shared_ptr<Font> font, const std::string& text, Color color = Color::WHITE);
		~Text();

		Text(const Text& other) = delete;
		Text(Text&& other) noexcept = delete;
		Text& operator=(const Text& other) = delete;
		Text& operator=(Text&& other) noexcept = delete;

		//Getters
		std::shared_ptr<Texture> getTexture() const;
		const std::string& getText() const;
		glm::vec2 getSize() const;
		Color getColor() const;

		TTF_GPUAtlasDrawSequence* getGPUDrawData() const;
		TTF_Text* getRendererDrawData() const;

		//Setters
		void setFont(const std::shared_ptr<Font>& font);
		void setColor(const Color& color);
		void setText(const std::string& text);
		void setWrappedWidth(int wrapped_width);

		void updateText(graphics::Renderer& renderer);
	private:
		void generateTextTexture(graphics::Renderer& renderer);

		std::string text;
		TTF_Text* m_text = nullptr;
	};

} // namespace graphics