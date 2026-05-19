#pragma once

#include <memory>

#include "glm/vec2.hpp"
#include "Enums.hpp"
#include "Window.hpp"

namespace graphics
{
    class Color;
    class RendererImpl;

    class Renderer
    {
    public:
        Renderer() = default;
        Renderer(Window& window);
        ~Renderer() = default;

        void create(Window& window);

        // Getters
        [[nodiscard]] float getZoom() const;
        [[nodiscard]] glm::vec2 getView() const;

        // Setters
        void setZoom(float zoom);
        void setView(const glm::vec2& view);
        void setRenderMove(RenderMode render_mode);

        void drawRectangle(float x, float y, float width, float height, const Color& color, RenderType render_type);

        void render();
    private:
        std::unique_ptr<RendererImpl> m_impl;
    };
}
