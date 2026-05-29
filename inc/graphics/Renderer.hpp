#pragma once

#include <memory>

#include "glm/vec2.hpp"
#include "Enums.hpp"
#include "Sprite.hpp"
#include "Surface.hpp"
#include "Text.hpp"
#include "Texture.hpp"
#include "TileMap.hpp"
#include "Window.hpp"

namespace graphics
{
    struct Color;
    class RendererImpl;

    class Renderer
    {
    public:
        Renderer();
        Renderer(Window& window);
        ~Renderer();

        void create(Window& window);

        std::shared_ptr<Texture> loadTexture
            (
                const Surface &surface,
                TextureScaleMode scale_mode = TextureScaleMode::LINEAR,
                TextureAddressMode address_mode = TextureAddressMode::CLAMP
            );
        
        std::shared_ptr<TileMap> loadTileMap(
            std::shared_ptr<Texture> texture,
            WorldSize world_size,
            TileSize tile_size,
            TileSizePixels tile_size_pixels, ChunkSize chunk_size
        );

        void shutdown();

        // Getters
        [[nodiscard]] float getZoom() const;
        [[nodiscard]] glm::vec2 getView() const;
        [[nodiscard]] const TextEngine& getTextEngine() const;
        [[nodiscard]] glm::ivec2 getRenderResolution() const;
        [[nodiscard]] glm::ivec2 getStandardWindowSize() const;

        // Setters
        void setZoom(float zoom);
        void setView(const glm::vec2& view);
        void setRenderMode(RenderMode render_mode);

        void drawRectangle(float x, float y, float width, float height, const Color& color, RenderType render_type);
        void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE);
        void drawText(const Text& text, float x, float y);
        void drawTileMap(std::shared_ptr<TileMap> tile_map, float x, float y);

        void startDrawing();
        void endDrawing();
    private:
        std::unique_ptr<RendererImpl> m_impl;
    };
}
