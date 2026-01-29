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
        RHIPipelineState(AetherEngine* engine, RHIGraphicsPipelineDesc desc)
            :m_pEngine(engine), m_GraphicDesc(desc)
        { m_ePipelineType = ERHIPipelineType::Graphics; }

        RHIPipelineState(AetherEngine* engine, RHIComputePipelineDesc desc)
            :m_pEngine(engine), m_ComputeDesc(desc)
        { m_ePipelineType = ERHIPipelineType::Compute; }

        RHIPipelineState(AetherEngine* engine, RHIRayTracingPipelineDesc desc)
            :m_pEngine(engine), m_RayTracingDesc(desc)
        { m_ePipelineType = ERHIPipelineType::RayTracing; }

        RHIPipelineState(AetherEngine* engine, RHIMeshShaderPipelineDesc desc)
            :m_pEngine(engine), m_MeshDesc(desc)
        { m_ePipelineType = ERHIPipelineType::Mesh; }


        virtual ~RHIPipelineState() = default;

        ERHIPipelineType GetType() const { return m_ePipelineType; }

        void SetGraphicDesc(RHIGraphicsPipelineDesc v)      { m_GraphicDesc = v; }
        void SetComputeDesc(RHIComputePipelineDesc v)       { m_ComputeDesc = v; }
        void SetRayTracingDesc(RHIRayTracingPipelineDesc v) { m_RayTracingDesc = v; }
        void SetMeshShaderDesc(RHIMeshShaderPipelineDesc v) { m_MeshDesc = v; }

        const RHIGraphicsPipelineDesc&      GetGraphicDesc()    const { return m_GraphicDesc; }
        const RHIComputePipelineDesc&       GetComputeDesc()    const { return m_ComputeDesc; }
        const RHIRayTracingPipelineDesc&    GetRayTracingDesc() const { return m_RayTracingDesc; }
        const RHIMeshShaderPipelineDesc&    GetMeshDesc()       const { return m_MeshDesc; }
        virtual RHIRootSignature*           GetRootSignature()  const { return nullptr;}

        virtual void* GetNativePSO() { return nullptr; }
        virtual size_t Hash();

    protected:
        AetherEngine*               m_pEngine = nullptr;
        size_t                      m_iPsoHashValue;
        ERHIPipelineType            m_ePipelineType;
        RHIGraphicsPipelineDesc     m_GraphicDesc;
        RHIComputePipelineDesc      m_ComputeDesc;
        RHIRayTracingPipelineDesc   m_RayTracingDesc;
        RHIMeshShaderPipelineDesc   m_MeshDesc;
    };

    
};