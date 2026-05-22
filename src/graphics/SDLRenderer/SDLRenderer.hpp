//
// Created by stasd on 5/21/26.
//

#pragma once
#include "graphics/Window.hpp"

namespace graphics
{
    class SDLRenderer
    {
    public:
        SDLRenderer() = default;
        SDLRenderer(Window& window);
        ~SDLRenderer();

        void create(Window& window);

        glm::ivec2 getWindowSize() const;

        template<typename Self>
        auto&& get(this Self&& self)
        { return self.m_renderer; }
    private:
       SDL_Renderer* m_renderer = nullptr;
        Window& window;
    };
}
