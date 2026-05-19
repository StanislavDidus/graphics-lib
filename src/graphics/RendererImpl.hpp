#pragma once

#include <memory>

#include "glm/vec2.hpp"
#include "graphics/Enums.hpp"
#include "graphics/Window.hpp"

namespace graphics
{
    class Color;

    class RendererImpl
    {
    public:
        RendererImpl() = default;
        virtual ~RendererImpl() = default;

        static std::unique_ptr<RendererImpl> createRenderer(Window& window);

        // Getters
        [[nodiscard]] virtual float getZoom() const = 0;
        [[nodiscard]] virtual glm::vec2 getView() const = 0;

        // Setters
        virtual void setZoom(float zoom) = 0;
        virtual void setView(const glm::vec2& view) = 0;
        virtual void setRenderMove(RenderMode render_mode) = 0;

        virtual void drawRectangle(float x, float y, float width, float height, const Color& color, RenderType render_type) = 0;

        virtual void draw() = 0;

    private:

    };
}