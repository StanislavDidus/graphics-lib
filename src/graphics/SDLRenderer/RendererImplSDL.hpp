//
// Created by stasd on 5/21/26.
//

#pragma once

#include "../RendererImpl.hpp"
#include "SDLRenderer.hpp"

namespace graphics
{
    class RendererImplSDL : public RendererImpl
    {
    public:
        RendererImplSDL(Window& window);
        ~RendererImplSDL() override = default;

        //void create(Window& window);

        std::shared_ptr<Texture> loadTexture(const Surface &surface, TextureScaleMode scale_mode, TextureAddressMode address_mode) override;

        float getZoom() const override;
        glm::vec2 getView() const override;

        void setZoom(float zoom) override;
        void setView(const glm::vec2 &view) override;
        void setRenderMode(RenderMode render_mode) override;

        void drawRectangle(float x, float y, float width, float height, const Color &color, RenderType render_type) override;
        void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE) override;
        void drawText(const Text& text, float x, float y, float width, float height, float angle, SDL_FlipMode flip) override;

        void draw() override;
    private:
        void setColor(const Color& color);
        void zoomRect(SDL_FRect& rect);

        SDLRenderer renderer;
        float zoom = 1.0f;
        glm::vec2 view = {0.0f, 0.0f};
        RenderMode render_mode = RenderMode::WORLD;
    };
}
