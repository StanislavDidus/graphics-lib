//
// Created by stasd on 5/21/26.
//

#include "RendererImplSDL.hpp"

#include <cmath>

#include "graphics/TextureSDL.hpp"

namespace graphics
{
    RendererImplSDL::RendererImplSDL(Window &window)
        : renderer(window)
    {
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

    float RendererImplSDL::getZoom() const
    {
        return zoom;
    }

    glm::vec2 RendererImplSDL::getView() const
    {
        return view;
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
        SDL_FRect src = sprite.getRect();
        SDL_FRect dst{x, y, width, height};
        SDL_Texture* texture = std::static_pointer_cast<TextureSDL>(sprite.getTexture())->get();
        SDL_RenderTexture(renderer.get(), texture, &src, &dst);

        // TODO: Apply texture address mode before drawing it
    }

    void RendererImplSDL::draw()
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
