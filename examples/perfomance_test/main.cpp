#include <iostream>

#include <graphics/graphics.hpp>

#include "glm/gtc/random.hpp"
#include <graphics/SpriteSheet.hpp>

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
    std::cout << "Program started." << std::endl;
    
    init();
    
    Window window{"Performance Test Example", 960, 540, SDL_WINDOW_RESIZABLE};
    GpuRenderer renderer{window};
    
    Surface surface{"assets/ice-cream.bmp"};
    std::shared_ptr<GpuTexture> texture = renderer.loadTexture(surface, "PointClamp");
    Sprite sprite{texture, SDL_FRect{0.0f, 0.0f, 100.0f, 100.0f}};
    
    Surface surface1{"assets/sky.png"};
    std::shared_ptr<GpuTexture> texture1 = renderer.loadTexture(surface1, "PointClamp");
    Sprite sprite1{texture1, SDL_FRect{0.0f, 0.0f, 64.0f, 80.0f}};
    
    // Test
    int number_objects = 10'000;
    std::vector<glm::vec2> positions;
    positions.reserve(number_objects);
    for (int i = 0; i < number_objects; ++i)
    {
        positions.emplace_back(glm::linearRand(0.0f,910.0f), glm::linearRand(0.0f, 490.0f));
    }
    
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
        
        /*
        int i = 0;
        for (const auto& position : positions)
        {
            if (i % 2 == 0)
                drawScaledSprite(renderer, sprite, position.x, position.y, 50.0f, 50.0f);
            else
                drawScaledSprite(renderer, sprite1, position.x, position.y, 50.0f, 50.0f);
            ++i;    
        }
        */
        
        drawRectangle(renderer, 0.0f, 0.0f, 50.0f, 50.0f, RenderType::NONE, Color::RED);
        /*
        drawRectangle(renderer, 0.0f, 0.0f, 50.0f, 50.0f, RenderType::FILL, Color::RED);
        drawScaledSprite(renderer, sprite1, 100.0f, 100.0f, 50.0f, 50.0f);
        drawRectangle(renderer, 0.0f, 200.0f, 50.0f, 50.0f, RenderType::FILL, Color::GREEN);
        */
        //drawScaledSprite(renderer, sprite, 0.0f, 0.0f, 50.0f, 50.0f);
        //drawScaledSprite(renderer, sprite1, 250.0f, 300.0f, 50.0f, 50.0f);
        
        renderer.render();
        
        double end_time = SDL_GetTicks();
        dt = (end_time - start_time) / 1000.0;
    }
    
    return 0;
}
