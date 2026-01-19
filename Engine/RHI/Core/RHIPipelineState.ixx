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
        RHIPipelineState(AetherEngine* engine, ERHIPipelineType type)
            :m_pEngine(engine), m_ePipelineType(type) {}
        virtual ~RHIPipelineState() = default;

        ERHIPipelineType                    GetType()           const { return m_ePipelineType; }

        void SetGraphicDesc(RHIGraphicsPipelineDesc v)      { m_GraphicDesc = v; }
        void SetComputeDesc(RHIComputePipelineDesc v)       { m_ComputDesc = v; }
        void SetRayTracingDesc(RHIRayTracingPipelineDesc v) { m_RayTracingDesc = v; }
        void SetMeshDesc(RHIMeshPipelineDesc v)             { m_MeshDesc = v; }

        const RHIGraphicsPipelineDesc&      GetGraphicDesc()    const { return m_GraphicDesc; }
        const RHIComputePipelineDesc&       GetComputeDesc()    const { return m_ComputDesc; }
        const RHIRayTracingPipelineDesc&    GetRayTracingDesc() const { return m_RayTracingDesc; }
        const RHIMeshPipelineDesc&          GetMeshDesc()       const { return m_MeshDesc; }
        virtual RHIRootSignature* GetRootSignature() const = 0;

        virtual void* GetNativePSO() = 0;
        virtual size_t Hash() = 0;

    protected:
        AetherEngine*               m_pEngine = nullptr;

        ERHIPipelineType            m_ePipelineType;
        RHIGraphicsPipelineDesc     m_GraphicDesc;
        RHIComputePipelineDesc      m_ComputDesc;
        RHIRayTracingPipelineDesc   m_RayTracingDesc;
        RHIMeshPipelineDesc         m_MeshDesc;
    };

    
};