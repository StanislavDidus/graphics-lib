#pragma once

#include <memory>

#include "glm/vec2.hpp"
#include "graphics/Enums.hpp"
#include "graphics/Sprite.hpp"
#include "graphics/Surface.hpp"
#include "graphics/Text.hpp"
#include "graphics/Texture.hpp"
#include "graphics/Window.hpp"
#include "graphics/TextEngine.hpp"
#include "graphics/TileMap.hpp"

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
        
        virtual std::shared_ptr<TileMap> loadTileMap
            (
                std::shared_ptr<Texture> texture,
                WorldSize world_size,
                TileSize tile_size,
                TileSizePixels tile_size_pixels, ChunkSize chunk_size
            ) = 0;

        virtual void shutdown() = 0;

        // Getters
        [[nodiscard]] virtual float getZoom() const = 0;
        [[nodiscard]] virtual glm::vec2 getView() const = 0;
        [[nodiscard]] virtual const TextEngine& getTextEngine() const = 0;
        [[nodiscard]] virtual glm::ivec2 getRenderResolution() const = 0;
        [[nodiscard]] virtual glm::ivec2 getStandardWindowSize() const = 0;

        // Setters
        virtual void setZoom(float zoom) = 0;
        virtual void setView(const glm::vec2& view) = 0;
        virtual void setRenderMode(RenderMode render_mode) = 0;

        virtual void drawRectangle(float x, float y, float width, float height, const Color& color, RenderType render_type) = 0;
        virtual void drawSprite(const Sprite& sprite, float x, float y, float width, float height, float angle = 0.0f, SDL_FlipMode flip = SDL_FLIP_NONE, const Color& color = Color::WHITE) = 0;
        virtual void drawText(const Text& text, float x, float y) = 0;
        virtual void drawTileMap(std::shared_ptr<TileMap> tile_map, float x, float y) = 0;

        virtual void startDrawing() = 0;
        virtual void endDrawing() = 0;
    };
}
