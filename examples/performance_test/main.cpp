#include <iostream>

#include <graphics/graphics.hpp>

#include "glm/gtc/random.hpp"

//#include "glm/gtc/random.hpp"
//#include <graphics/SpriteSheet.hpp>

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

        Window window{"Performance Test Example", 960, 540, SDL_WINDOW_RESIZABLE};
        //GpuRenderer renderer{window};
        Renderer renderer{window};

        // Test

        int number_objects = 10'000;
        std::vector<glm::vec2> positions;
        positions.reserve(number_objects);
        for (int i = 0; i < number_objects; ++i)
        {
            positions.emplace_back(glm::linearRand(0.0f,910.0f), glm::linearRand(0.0f, 490.0f));
        }

        auto texture = renderer.loadTexture(Surface{"assets/entities.png"});
        auto sprite = Sprite{texture};
        auto texture1 = renderer.loadTexture(Surface{"assets/sky.png"});
        auto sprite1 = Sprite{texture1};

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

            for (const auto& position : positions)
            {
                renderer.drawSprite(sprite, position.x, position.y, 50.0f, 50.0f);
            }

            renderer.endDrawing();

            double end_time = SDL_GetTicks();
            dt = (end_time - start_time) / 1000.0;
        }
    }

    TTF_Quit();
    SDL_Quit();

    std::cout << "Program stopped." << std::endl;
    
    return 0;
}
