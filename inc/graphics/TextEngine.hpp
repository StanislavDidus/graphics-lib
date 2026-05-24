//
// Created by stasd on 5/22/26.
//

#pragma once
#include <memory>

#include "SDL3_ttf/SDL_ttf.h"
#include "SDL3/SDL_gpu.h"

namespace graphics
{
   class TextEngine
   {
   public:
      TextEngine() = default;
      TextEngine(std::shared_ptr<SDL_GPUDevice> device);
      TextEngine(SDL_Renderer* renderer);
      ~TextEngine();

      TextEngine(const TextEngine& other) = delete;
      TextEngine(TextEngine&& other) noexcept = delete;
      TextEngine& operator=(const TextEngine& other) = delete;
      TextEngine& operator=(TextEngine&& other) noexcept = delete;

      template<typename Self>
      auto&& get(this Self&& self) { return self.m_text_engine; }
   private:
      enum class TEXT_ENGINE_CREATED_WITH
      {
         RENDERER,
         GPU_DEVICE,
      };

      TEXT_ENGINE_CREATED_WITH m_created_with;
      TTF_TextEngine* m_text_engine;
   };
}
