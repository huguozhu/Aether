module Aether:RHIPipelineState;
import :RHIPipelineState;
import :Engine;
import :EngineDefinition;
import :RHIStruct;
import :Format;
import <string>;

export namespace Aether
{
    
    size_t RHIPipelineState::Hash()
    {
        if (ERHIPipelineType::Graphics == m_ePipelineType)
            return m_GraphicDesc.GetHash();
        else if (ERHIPipelineType::Compute == m_ePipelineType)
            return m_ComputeDesc.GetHash();
        else if (ERHIPipelineType::RayTracing == m_ePipelineType)
            return m_RayTracingDesc.GetHash();
        else if (ERHIPipelineType::Mesh == m_ePipelineType)
            return m_MeshDesc.GetHash();
        else
            return 0;
    }
    
};