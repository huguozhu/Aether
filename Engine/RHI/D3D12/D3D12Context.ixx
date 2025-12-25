export module Aether.RHI:D3D12Context;

import :RHIContext;
import :D3D12Definition;

export namespace Aether
{
    class D3D12Context : public RHIContext
    {
    public:
        D3D12Context(AetherEngine* engine);
       
        //ID3D12GraphicsCommandListPtr d3dCmdList;
        //ID3D12CommandAllocatorPtr cmdAlloc;


    };

};