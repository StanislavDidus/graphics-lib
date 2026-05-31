#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "graphics/Renderer.hpp"
#include <graphics/exception.hpp>
#include <graphics/Window.hpp>
#include "graphics/Text.hpp"


namespace graphics
{
    /// Function initializes all necessary dependencies, including:
    /// SDL3, SDL_ttf, and SDL_shadercross
    /// Don't call if you want to initialize it yourself
    void init();
}