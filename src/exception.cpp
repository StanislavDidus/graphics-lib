#include <graphics/exception.hpp>
#include <format>

namespace graphics
{
    graphics::graphics_error::graphics_error(const std::string& message)
        : runtime_error(std::format("Graphic error occured: {}", message))
    {
    
    }
}
