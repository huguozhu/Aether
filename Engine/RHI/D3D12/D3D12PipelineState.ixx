export module Aether:D3D12PipelineState;
import :RHIPipelineState;
import :Engine;
import :EngineDefinition;
import :RHIStruct;
import :D3D12Definition;
import <string>;


export namespace Aether
{

    class D3D12PipelineState : public RHIPipelineState
    {
    public:
        D3D12PipelineState(AetherEngine* engine, ERHIPipelineType type);

        void* GetNativePSO();
        ID3D12PipelineState* GetD3D12PipelineState();

    private:
        AResult CreateGraphicsPipelineState();
        AResult CreateComputePipelineState();
        AResult CreateRayTracingPipelineState();
        AResult CreateMeshPipelineState();

    private:
        ID3D12PipelineStatePtr m_pPipelineState = nullptr;

    };

};