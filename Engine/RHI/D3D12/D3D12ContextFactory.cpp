module;
#include <windows.h>

module Aether:D3D12Context;
import :D3D12Context;
import :D3D12Definition;
import :D3D12Window;
import :D3D12Shader;
import :D3D12Mesh;
import :D3D12Resource;
import :D3D12CommandList;
import :D3D12RootSignature;
import :D3D12PipelineState;
import :D3D12Buffer;
import :D3D12Texture;
import :Log;
import :Utils;
import :EngineDefinition;
import :RHIFence;
import std;

#include "Utils/Macros.h"
namespace Aether
{
    RHIRootSignaturePtr D3D12Context::CreateRootSignarue(RHIRootSignatureDesc desc)
    {
        return MakeSharedPtr<D3D12RootSignature>(m_pEngine, desc);
    }
    RHIPipelineStatePtr D3D12Context::CreateGraphicPipelineState(RHIGraphicsPipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL( ((D3D12PipelineState*)pipeline.get())->CreateGraphicsPipelineState());
        }
        return pipeline;
    }
    RHIPipelineStatePtr D3D12Context::CreateComputePipelineState(RHIComputePipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL(((D3D12PipelineState*)pipeline.get())->CreateComputePipelineState());
        }
        return pipeline;
    }
    RHIPipelineStatePtr D3D12Context::CreateRayTracingPipelineState(RHIRayTracingPipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL(((D3D12PipelineState*)pipeline.get())->CreateRayTracingPipelineState());
        }
        return pipeline;
    }
    RHIPipelineStatePtr D3D12Context::CreateMeshShaderPipelineState(RHIMeshShaderPipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL(((D3D12PipelineState*)pipeline.get())->CreateMeshPipelineState());
        }
        return pipeline;
    }
    RHICommandListPtr D3D12Context::CreateGraphicCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Graphics);
    }
    RHICommandListPtr D3D12Context::CreateComputeCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Compute);
    }
    RHICommandListPtr D3D12Context::CreateCopyCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Copy);
    }
    RHICommandListPtr D3D12Context::CreateBundleCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Bundle);
    }






    RHIShaderPtr D3D12Context::CreateShader(EShaderStage stage, std::string const& name, std::string const& entry_func_name, const void* byteCode, size_t byteCodeSize)
    {
        return MakeSharedPtr<D3D12Shader>(m_pEngine, stage, name, entry_func_name, byteCode, byteCodeSize);
    }
    RHIMeshPtr D3D12Context::CreateMesh()
    {
        return MakeSharedPtr<D3D12Mesh>(m_pEngine);
    }
    RHITexturePtr D3D12Context::CreateTexture2D(ID3D12ResourcePtr const& tex)
    {
        return MakeSharedPtr<D3D12Texture2D>(m_pEngine, tex);
    }
    RHITexturePtr D3D12Context::CreateTexture2D(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_datas)
    {
        if (tex_desc.type != ETextureType::Tex2D || tex_desc.width < 0 || tex_desc.height < 0 || tex_desc.num_mips <= 0)
            return nullptr;
        D3D12Texture2DPtr tex = MakeSharedPtr<D3D12Texture2D>(m_pEngine, tex_desc);
        AResult ret = tex->Create(init_datas);
        return AETHER_CHECKFAILED(ret) ? nullptr : tex;
    }
    RHITexturePtr D3D12Context::CreateTexture3D(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_datas)
    {
        if (tex_desc.type != ETextureType::Tex3D || tex_desc.width < 0 || tex_desc.height < 0 || tex_desc.num_mips <= 0)
            return nullptr;
        D3D12Texture3DPtr tex = MakeSharedPtr<D3D12Texture3D>(m_pEngine, tex_desc);
        AResult ret = tex->Create(init_datas);
        return AETHER_CHECKFAILED(ret) ? nullptr : tex;
    }
    RHITexturePtr D3D12Context::CreateTextureCube(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_data)
    {
        if (tex_desc.type != ETextureType::Cube || tex_desc.width < 0 || tex_desc.height < 0 || tex_desc.num_mips <= 0)
            return nullptr;
        D3D12TextureCubePtr tex = MakeSharedPtr<D3D12TextureCube>(m_pEngine, tex_desc);
        AResult ret = tex->Create(init_data);
        return AETHER_CHECKFAILED(ret) ? nullptr : tex;
    }


    RHIBufferPtr D3D12Context::CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data)
    {
        RHIBufferPtr buf = MakeSharedPtr<D3D12Buffer>(m_pEngine, data_size, flags);
        buf->Create(data_size, data);
        return buf;
    }
    RHIBufferPtr D3D12Context::CreateVertexBuffer(uint32_t data_size, const void* data)
    {
        RHIBufferPtr buf = MakeSharedPtr<D3D12VertexBuffer>(m_pEngine, data_size);
        buf->Create(data_size, data);
        return buf;
    }
    RHIBufferPtr D3D12Context::CreateIndexBuffer(uint32_t data_size, const  void* data)
    {
        RHIBufferPtr buf = MakeSharedPtr<D3D12IndexBuffer>(m_pEngine, data_size);
        buf->Create(data_size, data);
        return buf;
    }
    RHIShaderResourceViewPtr D3D12Context::CreateBufferSrv(RHIBufferPtr const& buffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems)
    {
        return MakeSharedPtr<D3D12BufferSrv>(m_pEngine, buffer, format, first_elem, num_elems);
    }
    RHIUnorderedAccessViewPtr D3D12Context::CreateBufferUav(RHIBufferPtr const& buffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems)
    {
        return MakeSharedPtr<D3D12BufferUav>(m_pEngine, buffer, format, first_elem, num_elems);
    }

    RHIRenderTargetViewPtr D3D12Context::Create2DRenderTargetView(RHITexturePtr const& tex_2d, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level)
    {                      
        return MakeSharedPtr<D3D12Texture2DCubeRtv>(m_pEngine, tex_2d, first_array_index, array_size, mip_level);
    }                      
    RHIRenderTargetViewPtr D3D12Context::Create2DRenderTargetView(RHITexturePtr const& tex_cube, uint32_t array_index, ECubeFaceType face, uint32_t mip_level)
    {                      
        return MakeSharedPtr<D3D12TextureCubeFaceRtv>(m_pEngine, tex_cube, array_index, face, mip_level);
    }                      
    RHIRenderTargetViewPtr D3D12Context::Create3DRenderTargetView(RHITexturePtr const& tex_3d, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level)
    {                      
        return MakeSharedPtr<D3D12Texture3DRtv>(m_pEngine, tex_3d, array_index, first_slice, num_slices, mip_level);
    }                      
    RHIDepthStencilViewPtr D3D12Context::Create2DDepthStencilView(RHITexturePtr const& tex_2d, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level)
    {                      
        return MakeSharedPtr<D3D12Texture2DDsv>(m_pEngine, tex_2d, first_array_index, array_size, mip_level);
    }                      
    RHIDepthStencilViewPtr D3D12Context::Create2DDepthStencilView(RHITexturePtr const& tex_2d, uint32_t array_index, ECubeFaceType face, uint32_t mip_level)
    {
        return MakeSharedPtr<D3D12TextureCubeFaceDsv>(m_pEngine, tex_2d, array_index, face, mip_level);
    }

    RHIFrameBufferPtr D3D12Context::CreateRHIFrameBuffer()
    {
        return MakeSharedPtr<D3D12FrameBuffer>(m_pEngine);
    }

};