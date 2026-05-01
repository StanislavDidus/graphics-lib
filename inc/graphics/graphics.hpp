#pragma once

namespace graphics
{
    /// Function initializes all necessary dependencies, including:
    /// SDL3, SDL_ttf, and SDL_shadercross
    /// Don't call if you want to initiliaze it yourself
    static void init()
    {
        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        {
            std::cerr << "ERROR: Could not initialize SDL: " << SDL_GetError() << std::endl;
        }
    
        if (!TTF_Init())
        {
            std::cerr << "ERROR: Could not initialize TTF: " << SDL_GetError() << std::endl;
        }
        if (!SDL_ShaderCross_Init())
        {
	    
            std::cerr << "ERROR: Could not initialize SDL_ShaderCross: " << SDL_GetError() << std::endl;
        }
    }
}