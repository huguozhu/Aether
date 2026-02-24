export module Aether:RHIContext;

import :RHIStruct;
import :RHICommandList;
import :RHIResource;
import :Error;
import :Engine;
import :RHITexture;
import :RHIRootSignature;
import :RHIFrameBuffer;
import <string>;
import <vector>;

export namespace Aether
{
    class AetherEngine;
    class RHIFactory
    {
    public:
        virtual RHIRootSignaturePtr CreateRootSignarue(RHIRootSignatureDesc desc) { return nullptr; }
        virtual RHIPipelineStatePtr CreateGraphicPipelineState(RHIGraphicsPipelineDesc desc) { return nullptr; }
        virtual RHIPipelineStatePtr CreateComputePipelineState(RHIComputePipelineDesc desc) { return nullptr; }
        virtual RHIPipelineStatePtr CreateRayTracingPipelineState(RHIRayTracingPipelineDesc desc) { return nullptr; }
        virtual RHIPipelineStatePtr CreateMeshShaderPipelineState(RHIMeshShaderPipelineDesc desc) { return nullptr; }
        virtual RHICommandListPtr CreateGraphicCommandList() { return nullptr; }
        virtual RHICommandListPtr CreateComputeCommandList() { return nullptr; }
        virtual RHICommandListPtr CreateCopyCommandList() { return nullptr; }
        virtual RHICommandListPtr CreateBundleCommandList() { return nullptr; }
        virtual RHIShaderPtr CreateShader(EShaderStage stage, std::string const& name, std::string const& entry_func_name, const void* byteCode, size_t byteCodeSize) { return nullptr; }
        virtual RHIMeshPtr CreateMesh() = 0;
        virtual RHITexturePtr CreateTexture2D(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_datas = {}) = 0;
        virtual RHITexturePtr CreateTexture3D(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_datas = {}) = 0;
        virtual RHITexturePtr CreateTextureCube(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_data = {}) = 0;

        virtual RHIBufferPtr CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data = nullptr) = 0;
        virtual RHIBufferPtr CreateVertexBuffer(uint32_t data_size, const void* data = nullptr) = 0;
        virtual RHIBufferPtr CreateIndexBuffer(uint32_t data_size, const void* data = nullptr) = 0;

        virtual RHIShaderResourceViewPtr CreateBufferSrv(RHIBufferPtr const& buffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems) = 0;
        virtual RHIUnorderedAccessViewPtr CreateBufferUav(RHIBufferPtr const& buffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems) = 0;
        virtual RHIRenderTargetViewPtr Create2DRenderTargetView(RHITexturePtr const& tex_2d, uint32_t first_array_index = 0, uint32_t array_size = 1, uint32_t mip_level = 0) = 0;
        virtual RHIRenderTargetViewPtr Create2DRenderTargetView(RHITexturePtr const& tex_cube, uint32_t array_index, ECubeFaceType face, uint32_t mip_level) = 0;
        virtual RHIRenderTargetViewPtr Create3DRenderTargetView(RHITexturePtr const& tex_3d, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level) = 0;
        virtual RHIDepthStencilViewPtr Create2DDepthStencilView(RHITexturePtr const& tex_2d, uint32_t first_array_index = 0, uint32_t array_size = 1, uint32_t mip_level = 0) = 0;
        virtual RHIDepthStencilViewPtr Create2DDepthStencilView(RHITexturePtr const& tex_2d, uint32_t array_index, ECubeFaceType face, uint32_t mip_level) = 0;

        virtual RHIFrameBufferPtr CreateRHIFrameBuffer() = 0;
    };
    
    class RHIContext : public RHIFactory
    {
    public:
        struct RenderPassInfo
        {
            std::string name = "RenderPass";
            RHIFrameBuffer* fb = nullptr;
        };

        RHIContext(AetherEngine* engine)
            :m_pEngine(engine)
        {}
        virtual ~RHIContext() = default;

        virtual AResult Init() = 0;
        virtual AResult AttachNativeWindows(std::string const& name, void* native_wnd) = 0;
        virtual AResult SwapBuffers() = 0;

        virtual AResult BeginRenderPass(const RenderPassInfo& renderPassInfo) = 0;
        virtual AResult EndRenderPass() = 0;

        CapabilitySet const& GetCapabilitySet() const { return m_CapabilitySet; }
        virtual AResult CheckCapabilitySetSupport() { return A_Success; }

        RHIFrameBufferPtr GetScreenFrameBuffer() { return m_pScreenFB; }

        static constexpr uint32_t const NUM_BACK_BUFFERS = 2;

    protected:
        AetherEngine* m_pEngine = nullptr;

        CapabilitySet m_CapabilitySet;

        // 当前渲染状态缓存（用于减少重复设置）
        struct {
            void* pipelineState = nullptr;
            std::vector<RHIResource*> shaderResources[8];     // 各槽位资源
            std::vector<RHIBuffer*> constantBuffers[16];
            std::vector<void*> samplers[16];
            EResourceState renderTargetState = EResourceState::RenderTarget;
            EResourceState depthStencilState = EResourceState::DepthWrite;
        } currentState;

        RHIFrameBufferPtr   m_pScreenFB = nullptr;
    };

};