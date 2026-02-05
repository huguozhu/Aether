module Aether:Effect;
import :Effect;
import :Hash;
import :RHIContext;
import :RHIShader;
import :RHIStruct;
import :ResourceManager;
import :Technique;

#include "Utils/Macros.h"
namespace Aether
{
    AResult Effect::Initialize()
    {
        LoadDefaultVirtualTechniques();
        return A_Success;
    }

    void Effect::LoadDefaultVirtualTechniques()
    {        
        RHIRenderTargetDesc forwardRtvDesc = { 1, {PixelFormat::B8G8R8A8_UNORM }, PixelFormat::D16,};
        RHIRenderTargetDesc toneMappingRtvDesc = { 1, {PixelFormat::B8G8R8A8_UNORM }, PixelFormat::Unknown, };

        LoadGraphicsTechnique("SimpleRendering", &RHIRenderStateDesc::Default3D(),
            &forwardRtvDesc, "SimpleRenderingVS", "SimpleRenderingPS");
        LoadGraphicsTechnique("ForwardRenderingCommon", &RHIRenderStateDesc::Default3D(), 
            &forwardRtvDesc, "MeshRenderingVS", "ForwardRenderingCommonPS");
        LoadGraphicsTechnique("ToneMapping", &RHIRenderStateDesc::PostProcess(), 
            &toneMappingRtvDesc, "PostProcessVS", "ToneMappingPS");
    }

    RHIShader* Effect::CreateShader(EShaderStage stage, const ShaderResourcePtr& shaderRes)
    {
        auto shaderIt = m_Shaders.find(shaderRes->_name);
        if (shaderIt != m_Shaders.end())
            return shaderIt->second.get();

        RHIShaderPtr shader = m_pEngine->RHIContextInstance().CreateShader(stage, shaderRes->_name, 
            shaderRes->reflectInfo.entry_point, shaderRes->sourceCode, shaderRes->sourceCodeSize);
        if (!shader)
        {
            LOG_ERROR("create shader %s fail", shaderRes->_name.c_str());
            return nullptr;
        }

        if (shader->Stage() == EShaderStage::Compute)
        {
            shader->SetCsThreadsPerGroup(shaderRes->reflectInfo.block_size.x,
                shaderRes->reflectInfo.block_size.y,
                shaderRes->reflectInfo.block_size.z);
        }

        RHIShader* shader_ = shader.get();
        m_Shaders[shaderRes->_name] = std::move(shader);
        return shader_;
    }

    VirtualTechnique* Effect::GetVirtualTechnique(const std::string& name)
    {
        auto it = m_VirtualTechniques.find(name);
        if (it == m_VirtualTechniques.end())
            return nullptr;
        return it->second.get();
    }

    Technique* Effect::GetTechnique(const std::string& name, const std::vector<EffectPredefine>& user_predefines)
    {
        auto virtualTech = GetVirtualTechnique(name);
        if (!virtualTech)
            return nullptr;
        else
            return virtualTech->Concrete(user_predefines);
    }

    Technique* Effect::GetTechnique(const std::string& name)
    {
        return GetTechnique(name, NULL_PREDEFINES);
    }

    AResult Effect::LoadGraphicsTechnique(const std::string& name, 
        const RHIRenderStateDesc* pRsDesc, 
        const RHIRenderTargetDesc* pRtvDesc,
        const char* vertexShaderName,
        const char* pixelShaderName, 
        const char* geometryShaderName, 
        const char* hullShaderName, 
        const char* domainShaderName)
    {
        if (this->GetTechnique(name))
            return A_Success;

        VirtualTechniquePtrUnique virtualTech = MakeUniquePtr<VirtualTechnique>(m_pEngine);
        virtualTech->SetName(name);
        if (pRsDesc)
            virtualTech->SetRenderState(*pRsDesc);
        if (pRtvDesc)
            virtualTech->SetRtvDesc(*pRtvDesc);
        if (vertexShaderName)
            virtualTech->SetShaderName(EShaderStage::Vertex, vertexShaderName);
        if (pixelShaderName)
            virtualTech->SetShaderName(EShaderStage::Pixel, pixelShaderName);
        if (geometryShaderName)
            virtualTech->SetShaderName(EShaderStage::Geometry, geometryShaderName);
        if (hullShaderName)
            virtualTech->SetShaderName(EShaderStage::Hull, hullShaderName);
        if (domainShaderName)
            virtualTech->SetShaderName(EShaderStage::Domain, domainShaderName);
        AResult ret = virtualTech->Build();
        if (AETHER_CHECKFAILED(ret))
        {
            LOG_ERROR("load default VirtualTechnique %s fail", name);
            return ret;            
        }
        virtualTech->SetPipelineType(ERHIPipelineType::Graphics);
        m_VirtualTechniques[name] = std::move(virtualTech);
        return A_Success;
    }
};