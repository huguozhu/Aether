export module Aether:RHIPipelineState;
import :Engine;
import :EngineDefinition;
import :RHIStruct;
import :Format;
import <string>;

export namespace Aether
{
    class RHIRootSignature;
    class RHIShader;
    class RHIPipelineState
    {
    public:
        RHIPipelineState(AetherEngine* engine)
            :m_pEngine(engine){}
        virtual ~RHIPipelineState() = default;

        virtual ERHIPipelineType            GetType()           const { return m_ePipelineType; }
        const RHIGraphicsPipelineDesc&      GetGraphicDesc()    const { return m_GraphicDesc; }
        const RHIComputePipelineDesc&       GetComputeDesc()    const { return m_ComputDesc; }
        const RHIRayTracingPipelineDesc&    GetRayTracingDesc() const { return m_RayTracingDesc; }
        const RHIMeshPipelineDesc&          GetMeshDesc()       const { return m_MeshDesc; }
        virtual RHIRootSignature* GetRootSignature() const = 0;

        virtual void* GetNativePSO() = 0;
        virtual uint32_t Hash() = 0;

    protected:
        AetherEngine*               m_pEngine = nullptr;

        ERHIPipelineType            m_ePipelineType = ERHIPipelineType::Graphics;
        RHIGraphicsPipelineDesc     m_GraphicDesc;
        RHIComputePipelineDesc      m_ComputDesc;
        RHIRayTracingPipelineDesc   m_RayTracingDesc;
        RHIMeshPipelineDesc         m_MeshDesc;
    };

    
};