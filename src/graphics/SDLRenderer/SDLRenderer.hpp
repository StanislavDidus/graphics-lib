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

        SDLRenderer(const SDLRenderer &other) = delete;
        SDLRenderer(SDLRenderer &&other) noexcept = delete;
        SDLRenderer& operator=(const SDLRenderer& other) = delete;
        SDLRenderer& operator=(SDLRenderer&& other) noexcept = delete;

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
