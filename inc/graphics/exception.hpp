#pragma once

namespace graphics
{
    class graphics_error : public std::runtime_error
    {
    public:
        graphics_error() = default
        ~graphics_error() = default;
        
    private:
    };
}