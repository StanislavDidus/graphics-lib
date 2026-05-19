#pragma once

#include <vector>

#include "../RendererImpl.hpp"
#include <SDL3/SDL_gpu.h>

#include "glm/fwd.hpp"
#include "graphics/GpuRenderer.hpp"
#include "graphics/RenderBatch.hpp"
#include "graphics/Vertex.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/transform.hpp"

namespace graphics
{
    class RendererImplGPU : public RendererImpl
    {
    public:
        //RendererImplGPU() = default;
        RendererImplGPU(Window& window);
        ~RendererImplGPU() override = default;

        void create(Window& window);


        void createNewSampler(const std::string& name, SDL_GPUFilter filter, SDL_GPUSamplerMipmapMode mipmap, SDL_GPUSamplerAddressMode address_mode, std::optional<int> anisotropy = std::nullopt);

        // Getter
        [[nodiscard]] float getZoom() const override;
        [[nodiscard]] glm::vec2 getView() const override;

        // Setter
        void setZoom(float zoom) override;
        void setView(const glm::vec2 &view) override;
        void setRenderMove(RenderMode render_mode) override;

        void drawRectangle(float x, float y, float width, float height, const Color &color, RenderType render_type) override;

        void draw() override;

    private:
        void initSamplers();

        SDL_FRect getCameraRect() const;

        Window& window;
        std::shared_ptr<SDL_GPUDevice> device = nullptr;

        RenderMode render_mode = RenderMode::WORLD;

        std::vector<DrawObject> draw_buffer;
        std::vector<DrawObject> ui_draw_buffer;

        // <Render Parameters> //
        glm::vec2 view = {0.0f, 0.0f};
        float zoom = 1.0f;
        float angle = 0.0f; ///< Degrees.
        glm::mat4 world_matrix;
        Uint32 render_resolution_width;
        Uint32 render_resolution_height;

        std::unique_ptr<Batcher> batcher;
        std::unique_ptr<Batcher> ui_batcher;

        std::unique_ptr<WindowClaimer> window_claimer;
        std::shared_ptr<GpuGraphicsPipeline> tilemap_graphics_pipeline;
        std::shared_ptr<GpuGraphicsPipeline> line_graphics_pipeline;
        std::shared_ptr<GpuGraphicsPipeline> vertex_graphics_pipeline;
        std::shared_ptr<GpuGraphicsPipeline> texture_graphics_pipeline;

        std::unique_ptr<GpuShader> tilemap_vertex_shader;
        std::unique_ptr<GpuShader> vertex_shader;
        std::unique_ptr<GpuShader> fragment_shader;
        std::unique_ptr<GpuShader> texture_vertex_shader;
        std::unique_ptr<GpuShader> texture_fragment_shader;

        std::unordered_map<std::string, std::shared_ptr<GpuSampler>> samplers;
    };
}
