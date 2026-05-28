#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_shadercross/SDL_shadercross.h>

#include "graphics/Renderer.hpp"

#include <graphics/exception.hpp>
#include <graphics/Window.hpp>
#include "Text.hpp"

#include <format>

namespace graphics
{
    /// Function initializes all necessary dependencies, including:
    /// SDL3, SDL_ttf, and SDL_shadercross
    /// Don't call if you want to initiliaze it yourself
    static void init()
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            throw graphics_error{std::format("Could not initialize SDL3: {}", SDL_GetError())};
        }
    
        if (!TTF_Init())
        {
            throw graphics_error{std::format("Could not initialize SDL_tff: {}", SDL_GetError())};
        }
        
        if (!SDL_ShaderCross_Init())
        {
            throw graphics_error{std::format("Could not initialize SDL_shadercross: {}", SDL_GetError())};
        }
    }
}