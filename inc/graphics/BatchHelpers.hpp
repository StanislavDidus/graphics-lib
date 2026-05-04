#pragma once
#include "GpuBuffer.hpp"
#include "GpuTransferBuffer.hpp"

namespace graphics
{
   struct PushSpriteData
   {
      GpuBuffer& buffer;
      GpuTransferBuffer& transfer_buffer;
      size_t& offset;
   };
   
   struct RenderSpriteData
   {
      std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline;
      GpuBuffer& buffer;
      SpriteUniform& sprite_uniform;
      size_t& offset;
   };
   
   struct PushVertexData
   {
      GpuBuffer& buffer;
      GpuTransferBuffer& transfer_buffer;
      size_t& offset;
   };
   
   struct RenderVertexData
   {
      std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline;
      GpuBuffer& buffer;
      size_t& offset;
   };
   
   struct PushLineData
   {
      GpuBuffer& buffer;
      GpuTransferBuffer& transfer_buffer;
      size_t& offset;
   };
   
   struct RenderLineData
   {
      std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline;
      GpuBuffer& buffer;
      size_t& offset;
   };
   
   struct RenderChunkData
   {
      std::shared_ptr<GpuGraphicsPipeline> graphics_pipeline;
   };
};