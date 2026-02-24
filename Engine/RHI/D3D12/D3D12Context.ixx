export module Aether:D3D12Context;

import :RHIContext;
import :D3DDxgiHelper;
import :D3D12Definition;
import :D3D12Fence;
import :D3D12Window;
import :D3D12GpuMemoryAllocator;
import :D3D12GpuDescriptorAllocator;

export namespace Aether
{
    class D3D12Context : public RHIContext, public DxgiHelper
    {
    public:
        D3D12Context(AetherEngine* engine);
        ~D3D12Context() = default;

        ID3D12Device* GetD3D12Device() { return m_pDevice.Get(); }
        ID3D12Device5* GetD3D12Device5() { return m_pDevice5.Get(); }
        ID3D12CommandQueue* GetD3D12CommandQueue() { return m_pCommandQueue.Get(); }

        // Render Thread 
        ID3D12CommandAllocator* D3DRenderCmdAllocator() const;
        ID3D12GraphicsCommandList* D3DRenderCmdList() const;
        void CommitRenderCmd();
        void SyncRenderCmd();
        void ResetRenderCmd();

        // Load Thread 
        ID3D12CommandAllocator* D3DLoadCmdAllocator() const;
        ID3D12GraphicsCommandList* D3DLoadCmdList() const;
        void CommitLoadCmd();
        void SyncLoadCmd();
        void ResetLoadCmd();

        // RenderTarget / DepthStencil Descriptor
        D3D12GpuDescriptorBlock AllocRtvDescBlock(uint32_t size);
        void DeallocRtvDescBlock(D3D12GpuDescriptorBlock&& desc_block);
        void RenewRtvDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size);
        D3D12GpuDescriptorBlock AllocDsvDescBlock(uint32_t size);
        void DeallocDsvDescBlock(D3D12GpuDescriptorBlock&& desc_block);
        void RenewDsvDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size);
        D3D12GpuDescriptorBlock AllocCbvSrvUavDescBlock(uint32_t size);
        void DeallocCbvSrvUavDescBlock(D3D12GpuDescriptorBlock&& desc_block);
        void RenewCbvSrvUavDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size);
        D3D12GpuDescriptorBlock AllocDynamicCbvSrvUavDescBlock(uint32_t size);
        void DeallocDynamicCbvSrvUavDescBlock(D3D12GpuDescriptorBlock&& desc_block);
        void RenewDynamicCbvSrvUavDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size);
        D3D12GpuDescriptorBlock AllocSamplerDescBlock(uint32_t size);
        void DeallocSamplerDescBlock(D3D12GpuDescriptorBlock&& desc_block);
        void RenewSamplerDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size);

        // Memory Descriptor
        D3D12GpuMemoryBlock AllocUploadMemBlock(uint32_t size_in_bytes, uint32_t alignment);
        void DeallocUploadMemBlock(D3D12GpuMemoryBlock&& mem_block);
        void RenewUploadMemBlock(D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment);
        D3D12GpuMemoryBlock AllocReadbackMemBlock(uint32_t size_in_bytes, uint32_t alignment);
        void DeallocReadbackMemBlock(D3D12GpuMemoryBlock&& mem_block);
        void RenewReadbackMemBlock(D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment);

        std::vector<D3D12_RESOURCE_BARRIER>* FindResourceBarriers(ID3D12GraphicsCommandList* cmd_list, bool allow_creation);
        void FlushResourceBarriers(ID3D12GraphicsCommandList* cmd_list);
        void AddResourceBarrier(ID3D12GraphicsCommandList* cmd_list, std::span<D3D12_RESOURCE_BARRIER> barriers);
        void AddStallResource(ID3D12ResourcePtr const& resource);


        AResult Init() override;        
        AResult AttachNativeWindows(std::string const& name, void* native_wnd) override;
        AResult SwapBuffers() override;
        AResult BeginRenderPass(const RenderPassInfo& renderPassInfo) override;
        AResult EndRenderPass() override;
        AResult CheckCapabilitySetSupport() override;

        RHIRenderTargetViewPtr GetScreenRtv();


    public:
        // Factory Functions
        RHIRootSignaturePtr CreateRootSignarue(RHIRootSignatureDesc desc) override;
        RHIPipelineStatePtr CreateGraphicPipelineState(RHIGraphicsPipelineDesc desc) override;
        RHIPipelineStatePtr CreateComputePipelineState(RHIComputePipelineDesc desc) override;
        RHIPipelineStatePtr CreateRayTracingPipelineState(RHIRayTracingPipelineDesc desc) override;
        RHIPipelineStatePtr CreateMeshShaderPipelineState(RHIMeshShaderPipelineDesc desc) override;
        RHICommandListPtr CreateGraphicCommandList() override;
        RHICommandListPtr CreateComputeCommandList() override;
        RHICommandListPtr CreateCopyCommandList()  override;
        RHICommandListPtr CreateBundleCommandList()  override;
        RHIShaderPtr CreateShader(EShaderStage stage, std::string const& name, std::string const& entry_func_name, const void* byteCode, size_t byteCodeSize) override;
        RHIMeshPtr CreateMesh() override;

        RHITexturePtr CreateTexture2D(ID3D12ResourcePtr const& tex);
        RHITexturePtr CreateTexture2D(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_datas = {}) override;
        RHITexturePtr CreateTexture3D(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_datas = {}) override;
        RHITexturePtr CreateTextureCube(const RHITexture::Desc& tex_desc, std::span<BitmapBufferPtr> init_data = {}) override;

        RHIBufferPtr CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data = nullptr);
        RHIBufferPtr CreateVertexBuffer(uint32_t data_size, const void* data = nullptr);
        RHIBufferPtr CreateIndexBuffer(uint32_t data_size, const void* data = nullptr);

        RHIShaderResourceViewPtr CreateBufferSrv(RHIBufferPtr const& buffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems) override;
        RHIUnorderedAccessViewPtr CreateBufferUav(RHIBufferPtr const& buffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems) override;

        RHIRenderTargetViewPtr  Create2DRenderTargetView(RHITexturePtr const& tex_2d, uint32_t first_array_index = 0, uint32_t array_size = 1, uint32_t mip_level = 0) override;
        RHIRenderTargetViewPtr  Create2DRenderTargetView(RHITexturePtr const& tex_cube, uint32_t array_index, ECubeFaceType face, uint32_t mip_level) override;
        RHIRenderTargetViewPtr  Create3DRenderTargetView(RHITexturePtr const& tex_3d, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level) override;
        RHIDepthStencilViewPtr  Create2DDepthStencilView(RHITexturePtr const& tex_2d, uint32_t first_array_index = 0, uint32_t array_size = 1, uint32_t mip_level = 0) override;
        RHIDepthStencilViewPtr  Create2DDepthStencilView(RHITexturePtr const& tex_2d, uint32_t array_index, ECubeFaceType face, uint32_t mip_level) override;




    private:
        class PerThreadContext;
        PerThreadContext& CurThreadContext(bool is_render_context) const;
        void CommitCmd(PerThreadContext& context);
        void SyncCmd(PerThreadContext& context);
        void ResetCmd(PerThreadContext& context);

    private:
        /******************************************************************************
        * D3D12Context::PerThreadContext
        *******************************************************************************/
        class PerThreadContext
        {
        public:
            PerThreadContext(ID3D12Device* d3d_device, RHIFencePtr const& frame_fence);
            ~PerThreadContext();

            void CommitCmd(ID3D12CommandQueue* d3d_cmd_queue, uint32_t frame_index);
            void SyncCmd(uint32_t frame_index);
            void ResetCmd(uint32_t frame_index);
            void Reset(uint32_t frame_index);

            std::thread::id ThreadID() const { return m_ThreadId; }
            ID3D12CommandAllocator* D3DCmdAllocator(uint32_t frame_index) const;
            ID3D12GraphicsCommandList* D3DCmdList() const;

            uint64_t FrameFenceValue(uint32_t frame_index) const;

        private:
            struct PerThreadPerFrameContext
            {
                ID3D12CommandAllocatorPtr d3d_cmd_allocator = nullptr;
                uint64_t fence_value = 0;
            };

        private:
            std::thread::id m_ThreadId;
            std::array<PerThreadPerFrameContext, NUM_BACK_BUFFERS> m_vPerFrameContexts;
            ID3D12GraphicsCommandListPtr m_pD3dCmdList;
            std::weak_ptr<RHIFence> m_pFrameFence;
        };
        mutable std::vector<std::unique_ptr<PerThreadContext>> m_vRenderThreadCmdContexts;
        mutable std::vector<std::unique_ptr<PerThreadContext>> m_vLoadThreadCmdContexts;

        struct PerFrameContext
        {
        public:
            PerFrameContext() = default;
            ~PerFrameContext();

            void AddStallResource(ID3D12ResourcePtr const& resource);
            void ClearStallResources();

        private:
            std::vector<ID3D12ResourcePtr> m_vStallResources;
            std::mutex m_Mutex;
        };
        std::array<PerFrameContext, NUM_BACK_BUFFERS> m_vPerFrameContexts;
        uint32_t m_iCurFrameIndex = 0;
        RHIFencePtr m_pFrameFence = nullptr;
        uint64_t m_iFrameFenceValue = 0;

    private:
        ID3D12DevicePtr             m_pDevice = nullptr;
        ID3D12Device5Ptr            m_pDevice5 = nullptr;

        ID3D12CommandQueuePtr       m_pCommandQueue = nullptr;
        D3D12FencePtr               m_pFence = nullptr;

        D3D12WindowPtr              m_pPhysicalWindow;

        ID3D12DescriptorHeapPtr     m_pRtvHeap = nullptr;
        uint32_t                    m_iRtvDescSize;

        std::vector<std::pair<ID3D12GraphicsCommandList*, std::vector<D3D12_RESOURCE_BARRIER>>> m_vResBarriers;
        std::vector<ID3D12ResourcePtr> m_vStallResources;
        std::mutex m_Mutex;

        D3D12GpuDescriptorAllocatorPtr m_pRtvDescAllocator = nullptr;
        D3D12GpuDescriptorAllocatorPtr m_pDsvDescAllocator = nullptr;
        D3D12GpuDescriptorAllocatorPtr m_pCbvSrvUavDescAllocator = nullptr;
        D3D12GpuDescriptorAllocatorPtr m_pDynamicCbvSrvUavDescAllocator = nullptr;
        D3D12GpuDescriptorAllocatorPtr m_pSamplerDescAllocator = nullptr;
        D3D12GpuMemoryAllocatorPtr m_pUploadMemoryAllocator = nullptr;
        D3D12GpuMemoryAllocatorPtr m_pReadbackMemoryAllocator = nullptr;
    };

};