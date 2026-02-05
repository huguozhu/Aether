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


        D3D12GpuMemoryBlock AllocUploadMemBlock(uint32_t size_in_bytes, uint32_t alignment);
        void DeallocUploadMemBlock(D3D12GpuMemoryBlock&& mem_block);
        void RenewUploadMemBlock(D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment);
        D3D12GpuMemoryBlock AllocReadbackMemBlock(uint32_t size_in_bytes, uint32_t alignment);
        void DeallocReadbackMemBlock(D3D12GpuMemoryBlock&& mem_block);
        void RenewReadbackMemBlock(D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment);




        AResult Init() override;        
        AResult AttachNativeWindows(std::string const& name, void* native_wnd) override;
        AResult SwapBuffers() override;

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

        RHIBufferPtr CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data = nullptr);
        RHIBufferPtr CreateVertexBuffer(uint32_t data_size, const void* data = nullptr);
        RHIBufferPtr CreateIndexBuffer(uint32_t data_size, const void* data = nullptr);


        ID3D12Device* GetD3D12Device() { return m_pDevice.Get(); }
        ID3D12Device5* GetD3D12Device5() { return m_pDevice5.Get(); }

        ID3D12CommandQueue* GetD3D12CommandQueue() { return m_pCommandQueue.Get(); }
        AResult CheckCapabilitySetSupport() override;


    private:
        ID3D12DevicePtr             m_pDevice = nullptr;
        ID3D12Device5Ptr            m_pDevice5 = nullptr;

        ID3D12CommandQueuePtr       m_pCommandQueue = nullptr;
        D3D12FencePtr               m_pFence = nullptr;

        D3D12WindowPtr              m_pPhysicalWindow;

        ID3D12DescriptorHeapPtr     m_pRtvHeap = nullptr;
        uint32_t                    m_iRtvDescSize;

        D3D12GpuMemoryAllocatorPtr m_pUploadMemoryAllocator = nullptr;
        D3D12GpuMemoryAllocatorPtr m_pReadbackMemoryAllocator = nullptr;
    };

};