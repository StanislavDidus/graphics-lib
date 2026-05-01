#include <iostream>
#include <graphics/GpuRenderer.hpp>

#include <graphics/GpuRenderFunctions.hpp>
#include "SDL3_shadercross/SDL_shadercross.h"

using namespace graphics;

namespace graphics
{
    int MAX_SPRITES_RENDERED = 1000;
    int MAX_RECTANGLES_RENDERED = 1000;
    int MAX_LINES_RENDERED = 1000;
}

int main()
{
    std::cout << "Program started." << std::endl;
    
    
    Window window{"Empty Window Example", 960, 540, SDL_WINDOW_RESIZABLE};
    GpuRenderer renderer{window};
    
    SDL_Event event{};
    while (window)
    {
        window.pollEvent(event);
        
        renderer.update();
    }
    
    return 0;
}