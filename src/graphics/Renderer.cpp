#include "graphics/Renderer.hpp"
#include "graphics/RendererImpl.hpp"

namespace graphics
{
    Renderer::Renderer(Window &window)
    {
        create(window);
    }

    void Renderer::create(Window &window)
    {
       m_impl = RendererImpl::createRenderer(window);
    }
}
