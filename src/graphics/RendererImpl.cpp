#include "RendererImpl.hpp"

#include "GPURenderer/RendererImplGPU.hpp"
#include "SDLRenderer/RendererImplSDL.hpp"


#ifndef GRAPHICS_USE_SDL_RENDERER
using RendererType = graphics::RendererImplGPU;
#else
using RendererType = graphics::RendererImplSDL;
#endif

namespace graphics
{
    std::unique_ptr<RendererImpl> RendererImpl::createRenderer(Window& window)
    {
        return std::make_unique<RendererType>(window);
    }
}
