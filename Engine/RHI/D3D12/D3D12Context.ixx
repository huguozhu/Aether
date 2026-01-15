export module Aether:D3D12Context;

import :RHIContext;
import :D3DDxgiHelper;
import :D3D12Definition;
import :D3D12Fence;
import :D3D12Window;

export namespace Aether
{
    class D3D12Context : public RHIContext, public DxgiHelper
    {
    public:
        D3D12Context(AetherEngine* engine);
        ~D3D12Context() = default;

        AResult Init() override;        
        AResult AttachNativeWindows(std::string const& name, void* native_wnd) override;
        AResult SwapBuffers() override;

        ID3D12Device* GetD3D12Device() { return m_pDevice.Get(); }
        ID3D12CommandQueue* GetD3D12CommandQueue() { return m_pCommandQueue.Get(); }
        AResult CheckCapabilitySetSupport() override;


    private:
        ID3D12DevicePtr             m_pDevice = nullptr;
        ID3D12CommandQueuePtr       m_pCommandQueue = nullptr;
        D3D12FencePtr               m_pFence = nullptr;

        D3D12WindowPtr              m_pPhysicalWindow;

        ID3D12DescriptorHeapPtr     m_pRtvHeap = nullptr;
        uint32_t                    m_iRtvDescSize;
    };

};