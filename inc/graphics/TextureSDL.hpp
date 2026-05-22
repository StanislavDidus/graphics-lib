//
// Created by stasd on 5/19/26.
//

#pragma once

#include "Surface.hpp"
#include "Texture.hpp"
#include "graphics/SDLRenderer/SDLRenderer.hpp"
#include "SDL3/SDL_render.h"

namespace graphics
{
    class TextureSDL : public Texture
    {
    public:
        TextureSDL() = default;
        TextureSDL(SDLRenderer& renderer, const Surface& surface);
        ~TextureSDL() override;

        int width() const override { return m_surface.get()->w;}
        int height() const override { return m_surface.get()->h;}
        int pitch() const override { return m_surface.get()->pitch;}
        const void* pixels() const override { return m_surface.get()->pixels;}

        void setAddressMove(SDL_TextureAddressMode address_mode) { m_address_mode = address_mode; }

        template<typename Self>
        auto&& get(this Self&& self) { return self.m_texture; }
    private:
        SDL_Texture* m_texture = nullptr;
        Surface m_surface;
        SDL_TextureAddressMode m_address_mode = SDL_TEXTURE_ADDRESS_AUTO;
    };
}
