#include <iostream>

#include <graphics/graphics.hpp>

using namespace graphics;

namespace graphics
{
    int MAX_SPRITES_RENDERED = 1000;
    int MAX_RECTANGLES_RENDERED = 1000;
    int MAX_LINES_RENDERED = 1000;
}

float global_time = 0.0f;

int main()
{
    std::cout << "Program started." << std::endl;

    SDL_SetHint(SDL_HINT_GPU_DRIVER, "vulkan");
    
    init();
    
    Window window{"Empty Window Example", 960, 540, SDL_WINDOW_RESIZABLE};
    Renderer renderer{window};
    
    double dt  = 0.0;
    int tick = 0;
    SDL_Event event{};
    while (window.isOpen())
    {
        double start_time = SDL_GetTicks();
        window.pollEvent(event);
        
        // Calculate FPS
        ++tick;
        global_time += dt;
        if (global_time >= 1.0f)
        {
            tick /= global_time;
            global_time = 0.0f;
            
            std::cout << "FPS: " << tick << std::endl;
            
            tick = 0;
        }
        
        renderer.startDrawing();
        // Draw here 
        renderer.endDrawing();
        
        double end_time = SDL_GetTicks();
        dt = (end_time - start_time) / 1000.0;
    }
    
    return 0;
}