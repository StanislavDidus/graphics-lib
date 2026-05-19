//
// Created by stasd on 5/19/26.
//

#pragma once

namespace graphics
{
    ///
    /// Abstract class.
    /// Any other texture formats such as (GPUTextureSDL, TextureSDL) inherit from this class
    /// A pointer to this class is stored inside each sprite so that
    /// sprites can store any of the available texture formats SDL_GPUTexture, or SDL_Texture.
    ///
    class Texture
    {
    public:
        Texture() = default;
        virtual ~Texture() = default;

        virtual int width() const = 0;
        virtual int height() const = 0;;
        virtual int pitch() const = 0;;
        virtual const void* pixels() const = 0;;
    private:
    };
}