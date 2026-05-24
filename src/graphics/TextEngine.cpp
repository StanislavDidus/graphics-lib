//
// Created by stasd on 5/22/26.
//

#include "graphics/TextEngine.hpp"

#include <format>

#include "graphics/exception.hpp"

namespace graphics
{
    TextEngine::TextEngine(std::shared_ptr<SDL_GPUDevice> device)
    {
        m_text_engine = TTF_CreateGPUTextEngine(device.get());
        if (!m_text_engine)
            throw graphics_error{std::format("Error happened creating GPUTextEngine: {}", SDL_GetError())};
        m_created_with = TEXT_ENGINE_CREATED_WITH::GPU_DEVICE;
    }

    TextEngine::TextEngine(SDL_Renderer* renderer)
    {
        m_text_engine = TTF_CreateRendererTextEngine(renderer);
        if (!m_text_engine)
            throw graphics_error{std::format("Error happened creating RendererTextEngine: {}", SDL_GetError())};
        m_created_with = TEXT_ENGINE_CREATED_WITH::RENDERER;
    }

    TextEngine::~TextEngine()
    {
        if (m_created_with == TEXT_ENGINE_CREATED_WITH::GPU_DEVICE)
            TTF_DestroyGPUTextEngine(m_text_engine);
        else if (m_created_with == TEXT_ENGINE_CREATED_WITH::RENDERER)
            TTF_DestroyRendererTextEngine(m_text_engine);
    }
}
