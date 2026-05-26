#include <iostream>

#include <graphics/graphics.hpp>

using namespace graphics;

namespace graphics
{
    int MAX_SPRITES_RENDERED = 10'000;
    int MAX_RECTANGLES_RENDERED = 1000;
    int MAX_LINES_RENDERED = 1000;
}

float global_time = 0.0f;

int main()
{
    { // Game scope
        std::cout << "Program started." << std::endl;

        // Required for renderdoc on linux
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11");

        init();

        Window window{"TileMp Example", 960, 540, SDL_WINDOW_RESIZABLE};
        Renderer renderer{window};
        
        const auto& texture = renderer.loadTexture(Surface{"assets/tileset.png"});
        TileMap tile_map{texture, 24, 13, 20, 20, 16, 16};

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

            renderer.drawTileMap(tile_map, 0.0f, 0.0f);
            renderer.draw(); 

            double end_time = SDL_GetTicks();
            dt = (end_time - start_time) / 1000.0;
        }
    }

    TTF_Quit();
    SDL_Quit();

    std::cout << "Program stopped." << std::endl;
    
    return 0;
}
