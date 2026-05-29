#pragma once
#include "SDL3/SDL_gpu.h"

namespace graphics
{
    class WindowClaimer
    {
    public:
        WindowClaimer() = default;
        WindowClaimer(SDL_GPUDevice* device, SDL_Window* window) : device{device}, window{window} {}
        ~WindowClaimer()
        {
            if (device && window)
                SDL_ReleaseWindowFromGPUDevice(device, window);
        }   
    private:
        SDL_GPUDevice* device;
        SDL_Window* window;
    };
}
