#pragma once

namespace graphics
{
    enum class RenderType
    {
        NONE,
        FILL,
    };

    enum class RenderMode
    {
        WORLD,
        UI,
    };

    enum class TextureScaleMode
    {
        NEAREST,
        LINEAR,
        ANISOTROPIC,
    };

    enum class TextureAddressMode
    {
        CLAMP,
        WRAP,
    };
}