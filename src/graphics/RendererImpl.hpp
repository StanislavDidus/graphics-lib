#pragma once

#include <memory>

#include "glm/vec2.hpp"
#include "graphics/Enums.hpp"
#include "graphics/Sprite.hpp"
#include "graphics/Surface.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Window.hpp"

namespace graphics
{
    struct Color;

    class RendererImpl
    {
    public:
        RendererImpl() = default;
        virtual ~RendererImpl() = default;

        static std::unique_ptr<RendererImpl> createRenderer(Window& window);

        virtual std::shared_ptr<Texture> loadTexture
            (
                const Surface &surface,
                TextureScaleMode scale_mode = TextureScaleMode::LINEAR,
                TextureAddressMode address_mode = TextureAddressMode::CLAMP
            ) = 0;

        // Getters
        [[nodiscard]] virtual float getZoom() const = 0;
        [[nodiscard]] virtual glm::vec2 getView() const = 0;

        // Setters
        virtual void setZoom(float zoom) = 0;
        virtual void setView(const glm::vec2& view) = 0;
        virtual void setRenderMode(RenderMode render_mode) = 0;

        virtual void drawRectangle(float x, float y, float width, float height, const Color& color, RenderType render_type) = 0;
        virtual void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE) = 0;

        virtual void draw() = 0;

    private:

    };
}