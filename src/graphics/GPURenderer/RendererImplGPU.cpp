//
// Created by stasd on 5/19/26.
//

#include "RendererImplGPU.hpp"

#include "graphics/TextureSDL.hpp"
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

    	// Init Text Engine
    	text_engine = std::make_unique<TextEngine>(device);

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

    std::shared_ptr<Texture> RendererImplGPU::loadTexture(const Surface &surface, TextureScaleMode scale_mode,
                                                          TextureAddressMode address_mode)
    {
    	// Create texture with specified sampler
	    std::shared_ptr<GpuSampler> sampler = samplers.at(TextureSampler{scale_mode, address_mode});
    	std::shared_ptr<GpuTextureSDL> texture = std::make_shared<GpuTextureSDL>(device, surface, sampler);

    	// Upload texture on the GPU
    	GpuTransferBuffer texture_transfer_buffer{ device, static_cast<Uint32>(texture->width() * texture->height() * 4), SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD };

    	CommandBuffer command_buffer{device};

    	SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(command_buffer.get());

    	Uint8* texture_data = texture_transfer_buffer.map<Uint8>(false);
    	SDL_memcpy(texture_data, texture->pixels(), texture->width() * texture->height() * 4);
    	texture_transfer_buffer.unmap();

    	// Source
    	SDL_GPUTextureTransferInfo gpu_texture_transfer_info = {};
    	gpu_texture_transfer_info.transfer_buffer = texture_transfer_buffer.get();
    	gpu_texture_transfer_info.offset = 0;

    	// Destination
    	SDL_GPUTextureRegion gpu_texture_region = {};
    	gpu_texture_region.texture = texture->get();
    	gpu_texture_region.w = texture->width();
    	gpu_texture_region.h = texture->height();
    	gpu_texture_region.d = 1;

    	SDL_UploadToGPUTexture(copy_pass, &gpu_texture_transfer_info, &gpu_texture_region, false);
    	SDL_EndGPUCopyPass(copy_pass);

		return texture;
    }

    float RendererImplGPU::getZoom() const
    {
    	return zoom;
    }

    glm::vec2 RendererImplGPU::getView() const
    {
    	return view;
    }

    void RendererImplGPU::setZoom(float zoom)
    {
    	this->zoom = zoom;
    }

    void RendererImplGPU::setView(const glm::vec2 &view)
    {
    	this->view = view;
    }

    void RendererImplGPU::setRenderMode(RenderMode render_mode)
    {
    	this->render_mode = render_mode;
    }

    void RendererImplGPU::createNewSampler(TextureScaleMode scale_mode, TextureAddressMode address_mode)
    {
	    SDL_GPUFilter filter_ = scale_mode == TextureScaleMode::NEAREST ? SDL_GPU_FILTER_NEAREST : SDL_GPU_FILTER_LINEAR;
    	SDL_GPUSamplerAddressMode address_mode_ = address_mode == TextureAddressMode::CLAMP ? SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE : SDL_GPU_SAMPLERADDRESSMODE_REPEAT ;
    	SDL_GPUSamplerMipmapMode mipmap_mode_ = scale_mode == TextureScaleMode::NEAREST ? SDL_GPU_SAMPLERMIPMAPMODE_NEAREST : SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
    	std::optional<unsigned int> anisotropy_;
    	if (scale_mode == TextureScaleMode::ANISOTROPIC)
    		anisotropy_ = 4;
    	std::shared_ptr<GpuSampler> sampler = std::make_shared<GpuSampler>(device, filter_, mipmap_mode_, address_mode_, anisotropy_);
    	samplers[TextureSampler{scale_mode, address_mode}] = sampler;
    }

    SDL_FRect RendererImplGPU::getCameraRect() const
    {
    	return {};
    }

    void RendererImplGPU::drawRectangle(float x, float y, float width, float height, const Color &color,
                                        RenderType render_type)
    {
    	if (render_mode == RenderMode::WORLD)
    	{
    		if (render_type == RenderType::FILL)
    		{
    			draw_buffer.emplace_back(RectangleData{ glm::vec2{x,y},glm::vec2{width,height}, glm::vec4{color.r,color.g,color.b,color.a} });
    		}
    		else if (render_type == RenderType::NONE)
    		{
    			draw_buffer.emplace_back(LineData{ x,y,x + width,y, glm::vec4{color.r,color.g,color.b,color.a} });
    			draw_buffer.emplace_back(LineData{ x + width,y,x + width,y + height, glm::vec4{color.r,color.g,color.b,color.a} });
    			draw_buffer.emplace_back(LineData{ x + width,y + height,x,y + height, glm::vec4{color.r,color.g,color.b,color.a} });
    			draw_buffer.emplace_back(LineData{ x,y + height,x,y, glm::vec4{color.r,color.g,color.b,color.a} });
    		}
    	}
    	else if (render_mode == RenderMode::UI)
    	{
    		if (render_type == RenderType::FILL)
    		{
    			ui_draw_buffer.emplace_back(RectangleData{ glm::vec2{x,y},glm::vec2{width,height}, glm::vec4{color.r,color.g,color.b,color.a} });
    		}
    		else if (render_type == RenderType::NONE)
    		{
    			ui_draw_buffer.emplace_back(LineData{ x,y,x + width,y, glm::vec4{color.r,color.g,color.b,color.a} });
    			ui_draw_buffer.emplace_back(LineData{ x + width,y,x + width,y + height, glm::vec4{color.r,color.g,color.b,color.a} });
    			ui_draw_buffer.emplace_back(LineData{ x + width,y + height,x,y + height, glm::vec4{color.r,color.g,color.b,color.a} });
    			ui_draw_buffer.emplace_back(LineData{ x,y + height,x,y, glm::vec4{color.r,color.g,color.b,color.a} });

    		}
    	}
    }

    void RendererImplGPU::drawSprite(const Sprite &sprite, float x, float y, float width, float height, float angle,
	    SDL_FlipMode flip, const Color &color)
    {
    	SDL_FRect src = sprite.getRect();
    	SDL_FRect dst{ x, y, width, height };
    	std::shared_ptr<GpuTextureSDL> texture = std::static_pointer_cast<GpuTextureSDL>(sprite.getTexture());

    	if (render_mode == RenderMode::WORLD)
    	{
    		draw_buffer.emplace_back(
				std::in_place_type<GpuSprite>,
				texture,
				SpriteData
				{
				.pos_rot{dst.x, dst.y, 0.0f, angle},
				.size{dst.w, dst.h, 0.0f, 0.0f},
				.uv{src.x, src.y, src.w, src.h},
				.color{color.r, color.g, color.b, color.a},
				.flip{static_cast<float>(static_cast<unsigned int>(flip)), 0.0f, 0.0f, 0.0f}
				}
			);
    	}
    	else if (render_mode == RenderMode::UI)
    	{
    		ui_draw_buffer.emplace_back(
				std::in_place_type<GpuSprite>,
				texture,
				SpriteData
				{
				.pos_rot{dst.x, dst.y, 0.0f, angle},
				.size{dst.w, dst.h, 0.0f, 0.0f},
				.uv{src.x, src.y, src.w, src.h},
				.color{color.r, color.g, color.b, color.a},
				.flip{static_cast<float>(static_cast<unsigned int>(flip)), 0.0f, 0.0f, 0.0f}
				}
			);
    	}
    }

    void RendererImplGPU::drawText(const Text& text, float x, float y, float width, float height, float angle,
                                   SDL_FlipMode flip)
    {
    	SDL_FRect src = {0.0f, 0.0f, static_cast<float>(text.getTexture()->width()), static_cast<float>(text.getTexture()->height())};
    	SDL_FRect dst{ x, y, width, height };
    	std::shared_ptr<GpuTextureSDL> texture = std::static_pointer_cast<GpuTextureSDL>(text.getTexture());
    	const Color color = Color::WHITE;

    	if (render_mode == RenderMode::WORLD)
    	{
    		draw_buffer.emplace_back(
				std::in_place_type<GpuSprite>,
				texture,
				SpriteData
				{
				.pos_rot{dst.x, dst.y, 0.0f, angle},
				.size{dst.w, dst.h, 0.0f, 0.0f},
				.uv{src.x, src.y, src.w, src.h},
				.color{color.r, color.g, color.b, color.a},
				.flip{static_cast<float>(static_cast<unsigned int>(flip)), 0.0f, 0.0f, 0.0f}
				}
			);
    	}
    	else if (render_mode == RenderMode::UI)
    	{
    		ui_draw_buffer.emplace_back(
				std::in_place_type<GpuSprite>,
				texture,
				SpriteData
				{
				.pos_rot{dst.x, dst.y, 0.0f, angle},
				.size{dst.w, dst.h, 0.0f, 0.0f},
				.uv{src.x, src.y, src.w, src.h},
				.color{color.r, color.g, color.b, color.a},
				.flip{static_cast<float>(static_cast<unsigned int>(flip)), 0.0f, 0.0f, 0.0f}
				}
			);
    	}
    }

    void RendererImplGPU::draw()
    {
		CommandBuffer command_buffer{ device };

		SDL_GPUTexture* swapchain_texture;

		if (!SDL_WaitAndAcquireGPUSwapchainTexture(command_buffer.get(), window.get(), &swapchain_texture, &render_resolution_width, &render_resolution_height))
		{
			throw std::runtime_error{ std::format("SDL_WaitAndAcquireGPUSwapchainTexture failed: {}", SDL_GetError()) };
		}

		//.........................//
		//<Calculate object matrix>//
		//.........................//
		const auto& window_size = window.getWindowSize();
		const auto& view_position = view;
		glm::vec2 mid_screen = { window_size.x * 0.5f, window_size.y * 0.5f };

		glm::mat4 view_matrix = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ -view_position, 0.0f });
		glm::mat4 to_origin = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ -mid_screen, 0.0f });
		glm::mat4 from_origin = glm::translate(glm::mat4{ 1.0f }, glm::vec3{ mid_screen, 0.0f });

		glm::mat4 scale = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ zoom });
		glm::mat4 rotation = glm::rotate(glm::mat4{ 1.0f }, glm::radians(angle), glm::vec3{ 0.0f, 0.0f, 1.0f });

		glm::mat4 projection = glm::ortho(
			0.0f, static_cast<float>(window_size.x),
			static_cast<float>(window_size.y), 0.0f
		);
		//.........................//
		//<_______________________>//
		//.........................//

		glm::mat4 base_matrix = glm::transpose(projection);
		world_matrix = glm::transpose(projection * from_origin * rotation * scale * to_origin * view_matrix);

		if (swapchain_texture)
		{
			SDL_GPUColorTargetInfo target_info
			{
				.texture = swapchain_texture,
				.clear_color = {0.0f, 0.0f, 0.0f, 1.0f},
				.load_op = SDL_GPU_LOADOP_CLEAR,
				.store_op = SDL_GPU_STOREOP_STORE,
				.cycle = false
			};

			batcher->setWorldMatrix(world_matrix);
			for (const auto& draw_object : draw_buffer)
			{
				if (!batcher->canBatch(draw_object))
					batcher->closeBatch();
				batcher->addToBatch(draw_object);
			}

			ui_batcher->setWorldMatrix(base_matrix);
			for (const auto& draw_object : ui_draw_buffer)
			{
				if (!ui_batcher->canBatch(draw_object))
					ui_batcher->closeBatch();
				ui_batcher->addToBatch(draw_object);
			}

			SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(command_buffer.get());
			batcher->pushAllToGPU(copy_pass);
			ui_batcher->pushAllToGPU(copy_pass);
			SDL_EndGPUCopyPass(copy_pass);

			SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass(command_buffer.get(), &target_info, 1, nullptr);
			batcher->renderAll(render_pass, command_buffer);
			ui_batcher->renderAll(render_pass, command_buffer);
			SDL_EndGPURenderPass(render_pass);
		}

		command_buffer.submit();

		//draw_buffer.clear();
		draw_buffer.clear();
		ui_draw_buffer.clear();
    }

    void RendererImplGPU::initSamplers()
    {
    	createNewSampler(TextureScaleMode::NEAREST, TextureAddressMode::CLAMP);
    	createNewSampler(TextureScaleMode::NEAREST, TextureAddressMode::WRAP);
    	createNewSampler(TextureScaleMode::LINEAR, TextureAddressMode::CLAMP);
    	createNewSampler(TextureScaleMode::LINEAR, TextureAddressMode::WRAP);
    	createNewSampler(TextureScaleMode::ANISOTROPIC, TextureAddressMode::CLAMP);
    	createNewSampler(TextureScaleMode::ANISOTROPIC, TextureAddressMode::WRAP);

    	std::cout << "Samplers initialized." << std::endl;
    }
}

