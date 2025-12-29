export module Aether:D3D12Context;

import :RHIContext;
import :D3D12Definition;
import :D3DDxgiHelper;
import :D3D12Definition;

namespace Aether
{
    class D3D12Context : public RHIContext, public DxgiHelper
    {
    public:
        D3D12Context(AetherEngine* engine);
        AResult Init() override;

        ID3D12Device* GetD3D12Device() { return m_pDevice.Get(); }
        AResult CheckCapabilitySetSupport() override;

        //ID3D12GraphicsCommandListPtr d3dCmdList;
        //ID3D12CommandAllocatorPtr cmdAlloc;


        ID3D12DevicePtr                 m_pDevice = nullptr;

    };

};