//
// Created by stasd on 5/19/26.
//

#include "graphics/TextureSDL.hpp"
#include "graphics/Surface.hpp"

namespace graphics
{
    TextureSDL::TextureSDL(SDL_Renderer &renderer, const Surface &surface)
        : m_surface{surface}
    {
        m_texture = SDL_CreateTextureFromSurface(&renderer, surface.get());
    }

    graphics::TextureSDL::~TextureSDL()
    {
        if (m_texture)
        {
            SDL_DestroyTexture(m_texture);
        }
    }
}
