//
// Created by stasd on 5/19/26.
//

#include "RendererImplGPU.hpp"

#include "SDL3_shadercross/SDL_shadercross.h"

namespace graphics
{
    RendererImplGPU::RendererImplGPU(Window &window) : window{window}
    {
        create(window);
    }

    void RendererImplGPU::create(Window &window)
    {
		// Initialize ScreenSize uniform
		screen_size_uniform.width = window.getWindowSize().x;
		screen_size_uniform.height = window.getWindowSize().y;

		//Create GPU
		bool debug = false;
	#ifndef NDEBUG
		debug = true;
	#endif
		device = std::shared_ptr<SDL_GPUDevice>{ SDL_CreateGPUDevice(SDL_ShaderCross_GetHLSLShaderFormats(), debug, nullptr), SDL_DestroyGPUDevice};

		if (!device)
		{
			throw std::runtime_error{ std::format("Could not create GPU device: {}", SDL_GetError()) };
		}

		std::cout << "GPU device created." << std::endl;
		std::cout << std::format("Using {} GPU implementation.", SDL_GetGPUDeviceDriver(device.get())) << std::endl;

		if (!SDL_ClaimWindowForGPUDevice(device.get(), window.get()))
		{
			throw std::runtime_error{ std::format("SDL_ClaimWindowForGPUDevice failed: {}", SDL_GetError()) };
		}
		else
		{
			window_claimer = std::make_unique<WindowClaimer>(device.get(), window.get());
		}

		std::cout << "Window was successfully claimed for current GPU device." << std::endl;

		// Disable VSync
		SDL_SetGPUSwapchainParameters(device.get(), window.get(), SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_IMMEDIATE);

		// Init vertex shader
		tilemap_vertex_shader = std::make_unique<GpuShader>(device, "shaders/TileMap.vert.hlsl", 0, 2, 2, 0);
		vertex_shader = std::make_unique<GpuShader>(device, "shaders/VertexPositionMatrix.vert.hlsl", 0, 1, 0, 0);
		fragment_shader = std::make_unique<GpuShader>(device, "shaders/SolidColor.frag.hlsl", 0, 0, 0, 0);
		texture_vertex_shader = std::make_unique<GpuShader>(device, "shaders/PullSpriteBatch.vert.hlsl", 0, 2, 1, 0);
		texture_fragment_shader = std::make_unique<GpuShader>(device, "shaders/TextureQuadColor.frag.hlsl", 1, 0, 0, 0);

		std::cout << "Shaders initialized." << std::endl;

		initSamplers();

		// TileMap graphics pipeline
		{
			tilemap_graphics_pipeline = std::make_shared<GpuGraphicsPipeline>(device, window, *tilemap_vertex_shader, *texture_fragment_shader, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST);
		}

		// Line graphics pipeline
		{
			std::vector<SDL_GPUVertexBufferDescription> vertex_buffer_descriptions;
			vertex_buffer_descriptions.emplace_back(0, static_cast<Uint32>(sizeof(Vertex)), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0);

			std::vector<SDL_GPUVertexAttribute> vertex_attributes;
			vertex_attributes.emplace_back(0u, 0u, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0u);
			vertex_attributes.emplace_back(1u, 0u, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, static_cast<Uint32>(3 * sizeof(float)));

			line_graphics_pipeline = std::make_shared<GpuGraphicsPipeline>(device, window, *vertex_shader, *fragment_shader, SDL_GPU_PRIMITIVETYPE_LINELIST, vertex_buffer_descriptions, vertex_attributes);
		}

		// Vertex graphics pipeline
		{
			std::vector<SDL_GPUVertexBufferDescription> vertex_buffer_descriptions;
			vertex_buffer_descriptions.emplace_back(0, static_cast<Uint32>(sizeof(Vertex)), SDL_GPU_VERTEXINPUTRATE_VERTEX, 0);

			std::vector<SDL_GPUVertexAttribute> vertex_attributes;
			vertex_attributes.emplace_back(0u, 0u, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3, 0u);
			vertex_attributes.emplace_back(1u, 0u, SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4, static_cast<Uint32>(3 * sizeof(float)));

			vertex_graphics_pipeline = std::make_shared<GpuGraphicsPipeline>(device, window, *vertex_shader, *fragment_shader, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST, vertex_buffer_descriptions, vertex_attributes);

		}

		// TextureVertex graphics pipeline
		{
			texture_graphics_pipeline = std::make_shared<GpuGraphicsPipeline>(device, window, *texture_vertex_shader, *texture_fragment_shader, SDL_GPU_PRIMITIVETYPE_TRIANGLELIST);
		}

		batcher = std::make_unique<Batcher>(device, texture_graphics_pipeline, vertex_graphics_pipeline, line_graphics_pipeline, tilemap_graphics_pipeline);
		ui_batcher = std::make_unique<Batcher>(device, texture_graphics_pipeline, vertex_graphics_pipeline, line_graphics_pipeline, tilemap_graphics_pipeline);
	    }

    void RendererImplGPU::initSamplers()
    {
    	createNewSampler("PointClamp", SDL_GPU_FILTER_NEAREST, SDL_GPU_SAMPLERMIPMAPMODE_NEAREST, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE);
    	createNewSampler("PointWrap", SDL_GPU_FILTER_NEAREST, SDL_GPU_SAMPLERMIPMAPMODE_NEAREST, SDL_GPU_SAMPLERADDRESSMODE_REPEAT);
    	createNewSampler("LinearClamp", SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERMIPMAPMODE_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE);
    	createNewSampler("LinearWrap", SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERMIPMAPMODE_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_REPEAT);
    	createNewSampler("AnisotropicClamp", SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERMIPMAPMODE_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE, 4);
    	createNewSampler("AnisotropicWrap", SDL_GPU_FILTER_LINEAR, SDL_GPU_SAMPLERMIPMAPMODE_LINEAR, SDL_GPU_SAMPLERADDRESSMODE_REPEAT, 4);

    	std::cout << "Samplers initialized." << std::endl;
    }
}
