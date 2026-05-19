#include "RendererImpl.hpp"

#include "GPURenderer/RendererImplGPU.hpp"

using RendererType = graphics::RendererImplGPU;

namespace graphics
{
    std::unique_ptr<RendererImpl> RendererImpl::createRenderer(Window& window)
    {
        return std::make_unique<RendererType>(window);
    }
}