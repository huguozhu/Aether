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
        D3D12PipelineState(AetherEngine* engine, RHIGraphicsPipelineDesc desc);
        D3D12PipelineState(AetherEngine* engine, RHIComputePipelineDesc desc);
        D3D12PipelineState(AetherEngine* engine, RHIRayTracingPipelineDesc desc);
        D3D12PipelineState(AetherEngine* engine, RHIMeshShaderPipelineDesc desc);

        void* GetNativePSO();

    private:
        /* Ray Tracing Relative codes */        
        AResult CreateGraphicsPipelineState();
        AResult CreateComputePipelineState();
        AResult CreateRayTracingPipelineState();
        AResult CreateMeshPipelineState();

        // Ray Tracing Functions
        AResult RayTracing_CreateSubobjects();
        AResult RayTracing_AddDxilLibrarySubobject(RHIShader* shader);
        AResult RayTracing_AddHitGroupSubobject(const HitGroup& groups);
        AResult RayTracing_AddRootSignatureSubobject(RHIRootSignature* rootSig, bool isLocal);
        AResult RayTracing_AddShaderConfigSubobject();
        AResult RayTracing_AddPipelineConfigSubobject();
        AResult RayTracing_AddShaderAssociationSubobject(const ShaderAssociation& association);
        AResult RayTracing_CollectShaderIdentifiers();
        ID3D12StateObjectPtr m_pStateObject;
        ID3D12StateObjectPropertiesPtr m_pStateObjectProperties;

        std::vector<D3D12_STATE_SUBOBJECT> m_vSubobjects;
        std::vector<void*> m_vSubobjectData;  // 保持数据生命周期
        std::unordered_map<std::string, ShaderIdentifier> m_vShaderIdentifiers;

    private:
        ID3D12PipelineStatePtr m_pPipelineState = nullptr;

    };

};