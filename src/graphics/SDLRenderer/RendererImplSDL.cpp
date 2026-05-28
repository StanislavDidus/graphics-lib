//
// Created by stasd on 5/21/26.
//

#include "RendererImplSDL.hpp"

#include <cmath>

#include "TilemapSDL.hpp"
#include "graphics/TextEngine.hpp"
#include "graphics/TextureSDL.hpp"

namespace graphics
{
    RendererImplSDL::RendererImplSDL(Window &window)
        : renderer(window)
    {
        text_engine = std::make_unique<TextEngine>(renderer.get());
    }

    std::shared_ptr<Texture> RendererImplSDL::loadTexture(const Surface &surface, TextureScaleMode scale_mode,
                                                          TextureAddressMode address_mode)
    {
        auto texture = std::make_shared<TextureSDL>(renderer, surface);

        // Set texture scale mode
        SDL_ScaleMode scale_mode_ = scale_mode == TextureScaleMode::NEAREST ? SDL_SCALEMODE_NEAREST : SDL_SCALEMODE_LINEAR;
        SDL_SetTextureScaleMode(texture->get(), scale_mode_);

        // Save texture address mode
        // It will be applier before the texture is drawn
        SDL_TextureAddressMode address_mode_ = address_mode == TextureAddressMode::CLAMP ? SDL_TEXTURE_ADDRESS_CLAMP : SDL_TEXTURE_ADDRESS_WRAP;
        texture->setAddressMove(address_mode_);

        return texture;
    }

    std::shared_ptr<TileMap> RendererImplSDL::loadTileMap(std::shared_ptr<Texture> texture, WorldSize world_size,
                                                          TileSize tile_size, TileSizePixels tile_size_pixels, ChunkSize chunk_size)
    {
        std::shared_ptr<TileMap> tile_map = std::make_shared<TileMapSDL>(texture, world_size, tile_size, tile_size_pixels, chunk_size);
        return tile_map;
    }

    void RendererImplSDL::shutdown()
    {
    }

    float RendererImplSDL::getZoom() const
    {
        return zoom;
    }

    glm::vec2 RendererImplSDL::getView() const
    {
        return view;
    }

    const TextEngine& RendererImplSDL::getTextEngine() const
    {
        return *text_engine;
    }

    void RendererImplSDL::setZoom(float zoom)
    {
        this->zoom = zoom;
    }

    void RendererImplSDL::setView(const glm::vec2 &view)
    {
        this->view = view;
    }

    void RendererImplSDL::setRenderMode(RenderMode render_mode)
    {
        this->render_mode = render_mode;
    }

    void RendererImplSDL::drawRectangle(float x, float y, float width, float height, const Color &color,
        RenderType render_type)
    {
        setColor(color);
        SDL_FRect rect {x, y, width, height};

        if (render_mode == RenderMode::UI)
        {
            rect.x -= view.x;
            rect.y -= view.y;
        }

        zoomRect(rect);

        if (render_type == RenderType::FILL)
        {
            SDL_RenderFillRect(renderer.get(), &rect);
        }
        else if (render_type == RenderType::NONE)
        {
            SDL_RenderRect(renderer.get(), &rect);
        }
    }

    void RendererImplSDL::drawSprite(const Sprite &sprite, float x, float y, float width, float height, float angle,
        SDL_FlipMode flip, const Color &color)
    {
        // Get unormalized texture coordinates
        float texture_width = static_cast<float>(sprite.getTexture()->width());
        float texture_height = static_cast<float>(sprite.getTexture()->height());
        SDL_FRect src = sprite.getRect();
        src = {src.x * texture_width, src.y * texture_height, src.w * texture_width, src.h * texture_height };

        SDL_FRect dst{x, y, width, height};
        std::shared_ptr<Texture> texture = sprite.getTexture();
        drawTexture(texture, src, dst, angle, flip, color);
    }

    void RendererImplSDL::drawText(const Text& text, float x, float y)
    {
        TTF_DrawRendererText(text.getRendererDrawData(), x, y);
    }

    void RendererImplSDL::drawTexture(std::shared_ptr<Texture> texture, const SDL_FRect& src, const SDL_FRect& dst,
                                      float angle, SDL_FlipMode flip, const Color& color)
    {
        SDL_Texture* texture_ = std::static_pointer_cast<TextureSDL>(texture)->get();
        SDL_SetTextureColorMod(texture_, color.r, color.g, color.b);
        SDL_RenderTextureRotated(renderer.get(), texture_, &src, &dst, angle, nullptr, flip);
        SDL_SetTextureColorMod(texture_, 255, 255, 255);

        // TODO: Apply texture address mode before drawing it
    }
    
    void RendererImplSDL::drawTileMap(std::shared_ptr<TileMap> tile_map, float x, float y)
    {
        WorldSize world_size = tile_map->getWorldSize();
        TileSize tile_size = tile_map->getTilesize();
        TileSizePixels tile_size_pixels = tile_map->getTileSizePixels();
        ChunkSize chunk_size = tile_map->getChunkSize();
        
        float chunk_width_world = chunk_size.width * tile_size.width;
        float chunk_height_world = chunk_size.height * tile_size.height;
        
        float tile_width_pixels = static_cast<float>(tile_size_pixels.width);
        float tile_height_pixels = static_cast<float>(tile_size_pixels.height);
        
        std::shared_ptr<Texture> texture = tile_map->getTexture();
        for (int i = 0; const auto& chunk : std::static_pointer_cast<TileMapSDL>(tile_map)->getChunks())
        {
            int world_height_chunks = world_size.height / chunk_size.height;
            int chunk_x = i / world_height_chunks;
            int chunk_y = i % world_height_chunks;
            
            float offset_x = x + chunk_x * chunk_width_world;
            float offset_y = y + chunk_y * chunk_height_world;
            
            for (int j = 0; j < chunk_size.width; ++j)
            {
                for (int k = 0; k < chunk_size.height; ++k)
                {
                    int sprite_id = chunk.getGrid()[k * chunk_size.width + j];
                    float tile_x = j * tile_size.width;
                    float tile_y = k * tile_size.height;
                    
                    SDL_FRect src 
                    {
                       sprite_id * tile_width_pixels,
                        0.0f,
                        tile_width_pixels,
                        tile_height_pixels
                    };
                    SDL_FRect dst
                    {
                        tile_x + offset_x,
                        tile_y + offset_y,
                        static_cast<float>(tile_size.width),
                        static_cast<float>(tile_size.height)
                    };
                    drawTexture(texture, src, dst);
                }
            }
            
           ++i; 
        }
    }

    void RendererImplSDL::startDrawing()
    {
        setColor(Color::BLACK);
        SDL_RenderClear(renderer.get());
    }

    void RendererImplSDL::endDrawing()
    {
        SDL_RenderPresent(renderer.get());
    }

    void RendererImplSDL::setColor(const Color& color)
    {
        SDL_SetRenderDrawColor(renderer.get(), color.r, color.g, color.b, color.a);
    }

    void RendererImplSDL::zoomRect(SDL_FRect& rect)
	{
		const glm::vec2 mid_screen{
			static_cast<float>(renderer.getWindowSize().x) / 2.f,
			static_cast<float>(renderer.getWindowSize().y) / 2.f
		};

		rect.x -= mid_screen.x;
		rect.y -= mid_screen.y;

		rect.x *= zoom;
		rect.y *= zoom;
		rect.w *= zoom;
		rect.h *= zoom;

		rect.x += mid_screen.x;
		rect.y += mid_screen.y;

		rect.w += 1.f;
		rect.h += 1.f;

		rect.x = std::floor(rect.x);
		rect.y = std::floor(rect.y);
	}
}
