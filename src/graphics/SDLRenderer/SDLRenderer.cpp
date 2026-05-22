//
// Created by stasd on 5/21/26.
//

#include "SDLRenderer.hpp"
#include "SDL3/SDL_render.h"

namespace graphics
{
    SDLRenderer::SDLRenderer(Window &window)
        : window{window}
    {
        create(window);
    }

    SDLRenderer::~SDLRenderer()
    {
        SDL_DestroyRenderer(m_renderer);
    }

    void SDLRenderer::create(Window &window)
    {
        m_renderer = SDL_CreateRenderer(window.get(), nullptr);
    }

    glm::ivec2 SDLRenderer::getWindowSize() const
    {
        int w, h;
        SDL_GetWindowSize(window.get(), &w, &h);
        return {w, h};
    }
}
