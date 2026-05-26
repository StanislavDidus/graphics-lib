//
// Created by stasd on 5/21/26.
//

#pragma once

#include "../RendererImpl.hpp"
#include "../SDLRenderer.hpp"

namespace graphics
{
    class TextureSDL;

    class RendererImplSDL : public RendererImpl
    {
    public:
        RendererImplSDL(Window& window);
        ~RendererImplSDL() override = default;

        //void create(Window& window);

        std::shared_ptr<Texture> loadTexture(const Surface &surface, TextureScaleMode scale_mode, TextureAddressMode address_mode) override;

        void shutdown() override;

        [[nodiscard]] float getZoom() const override;
        [[nodiscard]] glm::vec2 getView() const override;
        [[nodiscard]] const TextEngine& getTextEngine() const override;

        void setZoom(float zoom) override;
        void setView(const glm::vec2 &view) override;
        void setRenderMode(RenderMode render_mode) override;

        void drawRectangle(float x, float y, float width, float height, const Color &color, RenderType render_type) override;
        void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE) override;
        void drawText(const Text& text, float x, float y) override;
        void drawTileMap(const TileMap& tile_map, float x, float y) override;

        void draw() override;
    private:
        void drawTexture(std::shared_ptr<Texture> texture, const SDL_FRect& src, const SDL_FRect& dst, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE);
        
        void setColor(const Color& color);
        void zoomRect(SDL_FRect& rect);

        SDLRenderer renderer;
        std::unique_ptr<TextEngine> text_engine;
        float zoom = 1.0f;
        glm::vec2 view = {0.0f, 0.0f};
        RenderMode render_mode = RenderMode::WORLD;
    };
}
