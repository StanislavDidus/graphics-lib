#include "graphics/Renderer.hpp"
#include "graphics/RendererImpl.hpp"

namespace graphics
{
    Renderer::Renderer()
    {
    }

    Renderer::Renderer(Window &window)
    {
        create(window);
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::create(Window &window)
    {
        m_impl = std::move(graphics::RendererImpl::createRenderer(window));
    }

    std::shared_ptr<Texture> Renderer::loadTexture(const Surface &surface, TextureScaleMode scale_mode,
                                                   TextureAddressMode address_mode)
    {
        return m_impl->loadTexture(surface, scale_mode, address_mode);
    }

    std::shared_ptr<TileMap> Renderer::loadTileMap(std::shared_ptr<Texture> texture, WorldSize world_size,
                                                    TileSize tile_size, TileSizePixels tile_size_pixels, ChunkSize chunk_size)
    {
        return m_impl->loadTileMap(texture, world_size, tile_size, tile_size_pixels, chunk_size);
    }

    void Renderer::shutdown()
    {
        m_impl->shutdown();
    }

    float Renderer::getZoom() const
    {
        return m_impl->getZoom();
    }

    glm::vec2 Renderer::getView() const
    {
        return m_impl->getView();
    }

    const TextEngine& Renderer::getTextEngine() const
    {
        return m_impl->getTextEngine();
    }

    void Renderer::setZoom(float zoom)
    {
        m_impl->setZoom(zoom);
    }

    void Renderer::setView(const glm::vec2 &view)
    {
        m_impl->setView(view);
    }

    void Renderer::setRenderMove(RenderMode render_mode)
    {
        m_impl->setRenderMode(render_mode);
    }

    void Renderer::drawRectangle(float x, float y, float width, float height, const Color &color,
        RenderType render_type)
    {
        m_impl->drawRectangle(x, y, width, height, color, render_type);
    }

    void Renderer::drawSprite(const Sprite &sprite, float x, float y, float width, float height, float angle,
        SDL_FlipMode flip, const Color &color)
    {
        m_impl->drawSprite(sprite, x, y, width, height, angle, flip, color);
    }

    void Renderer::drawText(const Text& text, float x, float y)
    {
        m_impl->drawText(text, x, y);
    }

    void Renderer::drawTileMap(std::shared_ptr<TileMap> tile_map, float x, float y)
    {
        m_impl->drawTileMap(tile_map, x, y);
    }

    void Renderer::startDrawing()
    {
        m_impl->startDrawing();
    }

    void Renderer::endDrawing()
    {
        m_impl->endDrawing();
    }
}
