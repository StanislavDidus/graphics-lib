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
        std::shared_ptr<TileMap> loadTileMap(std::shared_ptr<Texture> texture, WorldSize world_size, TileSize tile_size, TileSizePixels tile_size_pixels, ChunkSize chunk_size) override;

        void shutdown() override;

        [[nodiscard]] float getZoom() const override;
        [[nodiscard]] glm::vec2 getView() const override;
        [[nodiscard]] const TextEngine& getTextEngine() const override;
        [[nodiscard]] glm::ivec2 getStandardWindowSize() const override;
        [[nodiscard]] glm::vec2 getMouseScaledPosition(const glm::vec2& mouse_screen_position) override;

        void setZoom(float zoom) override;
        void setView(const glm::vec2 &view) override;
        void setRenderMode(RenderMode render_mode) override;

        void drawRectangle(float x, float y, float width, float height, const Color &color, RenderType render_type) override;
        void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE) override;
        void drawText(const Text& text, float x, float y) override;
        void drawTileMap(std::shared_ptr<TileMap> tile_map, float x, float y) override;

        void startDrawing() override;
        void endDrawing() override;
    private:
        void drawTexture(std::shared_ptr<Texture> texture, const SDL_FRect& src, const SDL_FRect& dst, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE);
        
        void setColor(const Color& color);
        void zoomRect(SDL_FRect& rect);
        SDL_FRect getCameraRect() const;

        SDLRenderer renderer;
        std::unique_ptr<TextEngine> text_engine;
        float zoom = 1.0f;
        glm::vec2 view = {0.0f, 0.0f};
        glm::ivec2 render_resolution;
        RenderMode render_mode = RenderMode::WORLD;
    };
}
