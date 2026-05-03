#pragma once

#include <stdexcept>
#include <string>

namespace graphics
{
    class graphics_error : public std::runtime_error
    {
    public:
        graphics_error(const std::string& message);
        ~graphics_error() override = default;
    };
}