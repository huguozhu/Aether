export module Aether:Technique;
import :Technique;
import :RHIContext;
import :Variable;
import :ResourceManager;
import :ShaderHelper;
import :Log;
import :Error;
import :Technique;
import :Effect;
import :Utils;


#include "Utils/Macros.h"
#define INVALID_BINDING_POINT  std::numeric_limits<uint32_t>::max()

namespace Aether
{
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // VirtualTechnique
    AResult VirtualTechnique::Build()
    {
        std::fill(m_metaShaderResources.begin(), m_metaShaderResources.end(), nullptr);
        m_predefines.clear();

        // collect all predefines in shaders
        for (uint32_t shader_type = 0; shader_type < (uint32_t)EShaderStage::Num; shader_type++)
        {
            std::string shader_name = m_shaderNames[shader_type];
            if (shader_name.empty())
                continue;

            MetaShaderResourcePtr metaShaderRes = m_pEngine->ResourceManagerInstance().LoadMetaShaderResource(shader_name);
            if (!metaShaderRes)
            {
                LOG_ERROR("load %s meta shader fail", shader_name.c_str());
                return ERR_INVALID_ARG;
            }
            m_metaShaderResources[shader_type] = metaShaderRes;

            for (auto& predefine : metaShaderRes->metaInfo.predefines)
            {
                bool found = false;
                for (auto& metaPredefine : m_predefines)
                {
                    if (metaPredefine.name == predefine.name)
                    {
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    m_predefines.push_back(predefine);
                }
            }
        }
        return A_Success;
    }

    static bool GetActivePredefineValue(const std::vector<EffectPredefine>& predefines, const std::string& activeName, std::string& activeValue)
    {
        for (auto& _predefine : predefines)
        {
            if (_predefine.name == activeName)
            {
                activeValue = _predefine.value;
                return true;
            }
        }
        return false;
    }

    static bool ActivePredefine(
        std::vector<MetaPredefine>  const& metaPredefines,
        std::vector<EffectPredefine>                const& providedPredefines,
        std::vector<EffectPredefine>& activedPredefines)
    {
        activedPredefines.resize(metaPredefines.size());
        for (size_t i = 0; i < activedPredefines.size(); i++)
        {
            activedPredefines[i].name = metaPredefines[i].name;
            if (!GetActivePredefineValue(providedPredefines, activedPredefines[i].name, activedPredefines[i].value))
            {
                activedPredefines.clear();
                return false;
            }
        }
        return true;
    }

    Technique* VirtualTechnique::Concrete(const std::vector<EffectPredefine>& predefines, const RHIRenderStateDesc& renderStateDesc)
    {
        std::vector<EffectPredefine> activedTechPredefines;
        bool actived = ActivePredefine(m_predefines, predefines, activedTechPredefines);
        if (!actived)
        {
            LOG_ERROR("concrete VirtualTechnique fail, the provided predefines are invalid");
            return nullptr;
        }

        std::string activedTechName = m_techName + GenerateSeedString(activedTechPredefines) + "_" + std::to_string(renderStateDesc.Hash());
        auto techIt = m_concreteTechs.find(activedTechName);
        if (techIt != m_concreteTechs.end())
            return techIt->second.get();

        TechniquePtr tech = MakeSharedPtr<Technique>(m_pEngine, this->shared_from_this());
        tech->SetPipelineType(m_ePipelineType);

        for (size_t stage = 0; stage < (uint32_t)EShaderStage::Num; stage++)
        {
            if (!m_metaShaderResources[stage])
                continue;

            std::vector<EffectPredefine> activedShaderPredefines;
            bool active = ActivePredefine(m_metaShaderResources[stage]->metaInfo.predefines, activedTechPredefines, activedShaderPredefines);
            if (!active)
            {
                LOG_ERROR("active shader predefine fail, it SHOULDN'T happen");
                return nullptr;
            }

            std::string activedShaderName = m_metaShaderResources[stage]->_name + GenerateSeedString(activedShaderPredefines);
            ShaderResourcePtr shaderRes = m_pEngine->ResourceManagerInstance().LoadShaderResource(activedShaderName);
            if (!shaderRes)
            {
                LOG_ERROR("no shader resource for %s", activedShaderName.c_str());
                return nullptr;
            }

            tech->SetShaderResource(static_cast<EShaderStage>(stage), shaderRes);
        }

        AResult ret = tech->Build();
        if (AETHER_CHECKFAILED(ret))
        {
            LOG_ERROR("Technique setup fail, VirtualTechnique: %s, with predefines", m_techName.c_str());
            for (auto idx = 0; idx != predefines.size(); idx++)
            {
                LOG_ERROR("  %s -> %s", predefines[idx].name.c_str(), predefines[idx].value.c_str());
            }
            return nullptr;
        }

        Technique* tech_ = tech.get();
        m_concreteTechs[activedTechName] = std::move(tech);
        return tech_;
    }

    Technique* VirtualTechnique::Concrete(const std::vector<EffectPredefine>& predefines)
    {
        return Concrete(predefines, m_RenderState);
    }

    Technique* VirtualTechnique::Concrete()
    {
        std::vector<EffectPredefine> null_predefines;
        return Concrete(null_predefines, m_RenderState);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Technique

    //RHIRenderStatePtr& Technique::GetRenderState()
    //{
    //    if (!m_RenderState)
    //    {
    //        m_RenderState = m_pEngine->RHIContextInstance().GetRenderState(m_RenderStateDesc);
    //    }
    //    return m_RenderState;
    //}

    static EffectDataType ConvertFromResourceType(ResourceType type)
    {
        switch (type)
        {
        case ResourceType::ConstantBuffer:
            return EffectDataType::ConstantBuffer;
        case ResourceType::Texture:
            return EffectDataType::Texture;
        case ResourceType::RWTexture:
            return EffectDataType::RWTexture;
        case ResourceType::Buffer:
            return EffectDataType::Buffer;
        case ResourceType::RWBuffer:
            return EffectDataType::RWBuffer;
        case ResourceType::Sampler:
            return EffectDataType::Sampler;
            //case ResourceType::SampledTexture:
            //    return EffectDataType::SampledTexture;
        default:
            return EffectDataType::Unknown;
        }
    }

    void Technique::CreateEffectVariable(Param& param)
    {
        switch (param.dataType)
        {
        case EffectDataType::ConstantBuffer:
        {
            param.variable = MakeUniquePtr<EffectVariableRHIGpuBuffer>();
            break;
        }
        case EffectDataType::Buffer:
        {
            param.variable = MakeUniquePtr<EffectVariableRHIShaderResourceView>();
            break;
        }
        case EffectDataType::RWBuffer:
        {
            param.variable = MakeUniquePtr<EffectVariableRHIUnorderedAccessView>();
            break;
        }
        case EffectDataType::Texture:
        case EffectDataType::RWTexture:
        {
            param.variable = MakeUniquePtr<EffectVariableRHITexture>();
            break;
        }
        case EffectDataType::Sampler:
        {
            param.variable = MakeUniquePtr<EffectVariableRHISampler>();
            break;
        }
        default:
            break;
        }
    }

    AResult Technique::Build()
    {
        switch (GetPipelineType())
        {
        case ERHIPipelineType::Graphics:    return BuildAsGraphicsPipeline();
        case ERHIPipelineType::Compute:     return BuildAsComputePipeline();
        case ERHIPipelineType::RayTracing:  return BuildAsRayTracingPipeline();
        case ERHIPipelineType::Mesh:        return BuildAsMeshShaderPipeline();
        default:
            LOG_ERROR("Technique::Build(), invalid PipelineType = %d", (uint32_t)GetPipelineType());
            return ERR_SYSTEM_ERROR;
        }
        return A_Success;
    }

    EShaderVisibility TranslateShaderVisibilityFromShaderStage(EShaderStage v)
    {
        switch (v)
        {
        case EShaderStage::Vertex:          return EShaderVisibility::Vertex;
        case EShaderStage::Pixel:           return EShaderVisibility::Pixel;
        case EShaderStage::Geometry:        return EShaderVisibility::Geometry;
        case EShaderStage::Domain:          return EShaderVisibility::Domain;
        case EShaderStage::Hull:            return EShaderVisibility::Hull;
        case EShaderStage::Compute:         return EShaderVisibility::Compute;
        case EShaderStage::Mesh:            return EShaderVisibility::Mesh;
        case EShaderStage::Amplification:   return EShaderVisibility::Amplification;
        default:                            return EShaderVisibility::All;
        }
    }

    void Technique::FillRootSignatureFromResource(RHIRootSignatureDesc& sig_desc)
    {
        for (size_t stage = 0; stage != (uint32_t)EShaderStage::Num; stage++)
        {
            auto& shaderRes = m_shaderRes[stage];
            if (!shaderRes)
                continue;
            sig_desc.debugName = GetName();
            for (auto& resource : shaderRes->reflectInfo.resources)
            {
                if (resource.type == ResourceType::Sampler)
                {
                    RHISamplerDesc sampler_desc = RHISamplerDesc::GetSamplerDescByName(resource.name);
                    sig_desc.staticSamplers.push_back(sampler_desc);
                }
                else
                {
                    RootParameter root_param;
                    if (resource.type == ResourceType::ConstantBuffer)
                    {
                        root_param.type = ERootParameterType::ConstantBuffer;
                        root_param.descriptor = { resource.id, resource.space };
                        root_param.visibility = TranslateShaderVisibilityFromShaderStage((EShaderStage)stage);
                    }
                    else if (resource.type == ResourceType::Buffer ||
                        resource.type == ResourceType::Texture)
                    {
                        root_param.type = ERootParameterType::ShaderResourceView;
                        root_param.descriptor = {};
                        root_param.visibility = TranslateShaderVisibilityFromShaderStage((EShaderStage)stage);
                    }
                    else if (resource.type == ResourceType::RWBuffer ||
                        resource.type == ResourceType::RWTexture)
                    {
                        root_param.type = ERootParameterType::UnorderedAccessView;
                        root_param.descriptor = {};
                        root_param.visibility = TranslateShaderVisibilityFromShaderStage((EShaderStage)stage);
                    }
                    sig_desc.parameters.push_back(root_param);
                }
            }
        }
    }
    AResult Technique::BuildAsGraphicsPipeline()
    {
        // Resources
        for (size_t stage = 0; stage != (uint32_t)EShaderStage::Num; stage++)
        {
            auto& shaderRes = m_shaderRes[stage];
            if (!shaderRes)
                continue;
            for (auto& resource : shaderRes->reflectInfo.resources)
            {
                auto paramIt = m_params.find(resource.name);
                if (paramIt != m_params.end())
                {
                    paramIt->second.bindings[stage] = resource.binding;
                    continue;
                }

                Param param;
                param.dataType = ConvertFromResourceType(resource.type);
                if (param.dataType == EffectDataType::Unknown)
                {
                    LOG_ERROR("invalid resource.type %d", resource.type);
                    return ERR_INVALID_ARG; // return? or keep going
                }                
                param.name = resource.name;
                //param.fallbackName = resource.fallback_name;
                param.bindings[stage] = resource.binding;
                CreateEffectVariable(param);
                m_params[param.name] = std::move(param);                
            }
        }

        RHIGraphicsPipelineDesc desc;
        // Step1: Shaders
        for (size_t stage = 0; stage != (uint32_t)EShaderStage::Num; stage++)
        {
            if (!m_shaderRes[stage])
                continue;
            RHIShader* shader = m_pEngine->EffectInstance().CreateShader((EShaderStage)stage, m_shaderRes[stage]);
            if (shader)
            {
                if ((uint32_t)EShaderStage::Vertex == stage)
                    desc.vertexShader = shader;
                else if ((uint32_t)EShaderStage::Pixel == stage)
                    desc.pixelShader = shader;
                else if ((uint32_t)EShaderStage::Geometry == stage)
                    desc.geometryShader = shader;
                else if ((uint32_t)EShaderStage::Hull == stage)
                    desc.hullShader = shader;
                else if ((uint32_t)EShaderStage::Domain == stage)
                    desc.domainShader = shader;
            }
        }
        // Step2: Render States
        desc.renderState = m_pVirtualTechnique->GetRenderState();
        
        // Step3: Input Elements
        if (m_shaderRes[(uint32_t)EShaderStage::Vertex])
        {
            ShaderResourcePtr& shaderRes = m_shaderRes[(uint32_t)EShaderStage::Vertex];
            for (SignatureParameter& signature : shaderRes->reflectInfo.input_signatures)
            {
                RHIInputElement element;
                element.semanticName = signature.semantic;
                element.semanticIndex = signature.semantic_index;
                element.inputSlot = signature.location;
                desc.inputElements.push_back(element);
            }
        }

        // Step4: Rtv
        desc.rtvDesc = m_pVirtualTechnique->GetRtvDesc();

        // Step5: Roo Signature
        RHIRootSignatureDesc sig_desc;
        FillRootSignatureFromResource(sig_desc);
        RHIRootSignaturePtr root_sig = m_pEngine->RHIContextInstance().CreateRootSignarue(sig_desc);
        desc.rootSignature = root_sig.get();
        desc.debugName = GetName();

        m_pPipelineState = m_pEngine->RHIContextInstance().CreateGraphicPipelineState(desc);
        m_pCommandList = m_pEngine->RHIContextInstance().CreateGraphicCommandList();
        return A_Success;
    }
    AResult Technique::BuildAsComputePipeline()
    {
        RHIComputePipelineDesc desc;
        RHIShader* shader = m_pEngine->EffectInstance().CreateShader(EShaderStage::Compute, m_shaderRes[(uint32_t)EShaderStage::Compute]);
        if (shader)
        {
            desc.computeShader = shader;
            desc.debugName = GetName();
        }
        RHIPipelineStatePtr pPipeline = m_pEngine->RHIContextInstance().CreateComputePipelineState(desc);
        return A_Success;
    }
    AResult Technique::BuildAsRayTracingPipeline()
    {
        //RHIRayTracingPipelineDesc desc;
        //for (size_t stage = 0; stage != (uint32_t)EShaderStage::Num; stage++)
        //{
        //    if (!m_shaderRes[stage])
        //        continue;
        //    RHIShader* shader = m_pEngine->EffectInstance().CreateShader((EShaderStage)stage, m_shaderRes[stage]);
        //    if (shader)
        //    {
        //        if ((uint32_t)EShaderStage::Mesh == stage)
        //            desc.meshShader = shader;
        //        else if ((uint32_t)EShaderStage::Amplification == stage)
        //            desc.amplification = shader;
        //        else if ((uint32_t)EShaderStage::Pixel == stage)
        //            desc.pixelShader = shader;
        //    }
        //}
        return A_Success;
    }
    AResult Technique::BuildAsMeshShaderPipeline()
    {
        RHIMeshShaderPipelineDesc desc;
        for (size_t stage = 0; stage != (uint32_t)EShaderStage::Num; stage++)
        {
            if (!m_shaderRes[stage])
                continue;
            RHIShader* shader = m_pEngine->EffectInstance().CreateShader((EShaderStage)stage, m_shaderRes[stage]);
            if (shader)
            {
                if ((uint32_t)EShaderStage::Mesh == stage)
                    desc.meshShader = shader;
                else if ((uint32_t)EShaderStage::Amplification == stage)
                    desc.amplificationShader = shader;
                else if ((uint32_t)EShaderStage::Pixel == stage)
                    desc.pixelShader = shader;
            }
        }

        return A_Success;
    }
    AResult Technique::Render(RHIMeshPtr const& mesh)
    {
        //this->Commit();
        //AResult ret;
        AETHER_RETIF_FAIL(m_pCommandList->Render(mesh));
        //this->Uncommit();
        //return ret;
        return A_Success;
    }

    //void Technique::Dispatch(uint32_t x, uint32_t y, uint32_t z)
    //{
    //    Commit();
    //    m_pContext->RHIContextInstance().Dispatch(m_pProgram.get(), x, y, z);
    //    Uncommit();
    //}
    //void Technique::DispatchIndirect(RHIBufferPtr indirectBuf)
    //{
    //    Commit();
    //    m_pContext->RHIContextInstance().DispatchIndirect(m_pProgram.get(), indirectBuf);
    //    Uncommit();
    //}
    //void Technique::DrawIndirect(RHIBufferPtr indirectBuf, MeshTopologyType type)
    //{
    //    RHIContext& rc = m_pContext->RHIContextInstance();
    //    RHIRenderStatePtr rs = this->GetRenderState();

    //    Commit();
    //    rc.DrawIndirect(m_pProgram.get(), rs, indirectBuf, type);
    //    Uncommit();
    //}
    //void Technique::DrawInstanced(MeshTopologyType type, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation)
    //{
    //    RHIContext& rc = m_pContext->RHIContextInstance();
    //    RHIRenderStatePtr rs = this->GetRenderState();

    //    Commit();
    //    rc.DrawInstanced(m_pProgram.get(), rs, type, vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
    //    Uncommit();
    //}
    AResult Technique::Commit()
    {
        RHIContext& rc = m_pEngine->RHIContextInstance();

        //for (auto& paramPair : m_params)
        //{
        //    auto& param = paramPair.second;
        //    for (size_t stage = 0; stage != (uint32_t)EShaderStage::Num; stage++)
        //    {
        //        if (param.bindings[stage] == INVALID_BINDING_POINT)
        //            continue;

        //        switch (param.dataType)
        //        {
        //        case EffectDataType::ConstantBuffer:
        //        {
        //            RHIBufferPtr rb;
        //            param.variable->Value(rb);
        //            if (rb)
        //                rc.BindConstantBuffer((ShaderType)stage, param.bindings[stage], rb.get(), param.fallbackName.c_str());
        //            else
        //                LOG_WARNING("param %s has no resource binding", param.name.c_str());
        //            break;
        //        }
        //        case EffectDataType::Buffer:
        //        {
        //            RHIShaderResourceViewPtr v;
        //            param.variable->Value(v);
        //            if (v)
        //                rc.BindRHISrv((ShaderType)stage, param.bindings[stage], v.get(), param.name.c_str());
        //            else
        //                LOG_WARNING("param %s has no resource binding", param.name.c_str());
        //            break;
        //        }
        //        case EffectDataType::RWBuffer:
        //        {
        //            RHIUnorderedAccessViewPtr v;
        //            param.variable->Value(v);
        //            if (v)
        //                rc.BindRHIUav((ShaderType)stage, param.bindings[stage], v.get(), param.name.c_str());
        //            else
        //                LOG_WARNING("param %s has no resource binding", param.name.c_str());
        //            break;
        //        }
        //        default:
        //        {
        //            if (m_pContext->GetRHIType() != RHIType::GLES)
        //            {
        //                switch (param.dataType)
        //                {
        //                case EffectDataType::Texture:
        //                {
        //                    RHITexturePtr tex;
        //                    param.variable->Value(tex);
        //                    if (tex)
        //                        rc.BindTexture((ShaderType)stage, param.bindings[stage], tex.get(), param.name.c_str());
        //                    else
        //                        LOG_WARNING("param %s has no resource binding", param.name.c_str());
        //                    break;
        //                }
        //                case EffectDataType::RWTexture:
        //                {
        //                    RHITexturePtr tex;
        //                    param.variable->Value(tex);
        //                    if (tex)
        //                        rc.BindRWTexture((ShaderType)stage, param.bindings[stage], tex.get(), param.name.c_str());
        //                    else
        //                        LOG_WARNING("param %s has no resource binding", param.name.c_str());
        //                    break;
        //                }
        //                case EffectDataType::Sampler:
        //                {
        //                    RHISamplerPtr sampler;
        //                    param.variable->Value(sampler);
        //                    if (!sampler)
        //                    {
        //                        sampler = rc.GetSampler(SamplerDesc::GetSamplerDescByName(param.name));
        //                        *(param.variable) = sampler;
        //                    }
        //                    rc.BindSampler((ShaderType)stage, param.bindings[stage], sampler.get(), param.name.c_str());
        //                    break;
        //                }
        //                default:
        //                    break;
        //                }
        //            }
        //            else
        //            {
        //                if (param.dataType == EffectDataType::SampledTexture)
        //                {
        //                    auto texIt = m_params.find(param.textureParamName);
        //                    if (texIt != m_params.end())
        //                    {
        //                        RHITexturePtr tex = nullptr;
        //                        texIt->second.variable->Value(tex);
        //                        rc.BindTexture((ShaderType)stage, param.bindings[stage], tex.get(), param.name.c_str());
        //                    }
        //                }
        //            }
        //        }
        //        }
        //    }
        //}
        return A_Success;
    }

    //void Technique::Uncommit()
    //{
    //    if (m_pContext->GetRHIType() != RHIType::D3D11)
    //        return;

    //    RHIContext& rc = m_pContext->RHIContextInstance();
    //    for (auto& paramPair : m_params)
    //    {
    //        auto& param = paramPair.second;
    //        for (size_t stage = 0; stage != SHADER_STAGE_COUNT; stage++)
    //        {
    //            if (param.bindings[stage] == INVALID_BINDING_POINT)
    //                continue;

    //            switch (param.dataType)
    //            {
    //            case EffectDataType::ConstantBuffer:
    //            {
    //                rc.BindConstantBuffer((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                break;
    //            }
    //            case EffectDataType::Buffer:
    //            {
    //                rc.BindRHISrv((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                break;
    //            }
    //            case EffectDataType::RWBuffer:
    //            {
    //                rc.BindRHIUav((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                break;
    //            }
    //            default:
    //            {
    //                if (m_pContext->GetRHIType() != RHIType::GLES)
    //                {
    //                    switch (param.dataType)
    //                    {
    //                    case EffectDataType::Texture:
    //                    {
    //                        rc.BindTexture((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                        break;
    //                    }
    //                    case EffectDataType::RWTexture:
    //                    {
    //                        rc.BindRWTexture((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                        break;
    //                    }
    //                    case EffectDataType::Sampler:
    //                    {
    //                        rc.BindSampler((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                        break;
    //                    }
    //                    default:
    //                        break;
    //                    }
    //                }
    //                else
    //                {
    //                    if (param.dataType == EffectDataType::SampledTexture)
    //                    {
    //                        rc.BindTexture((ShaderType)stage, param.bindings[stage], nullptr, nullptr);
    //                    }
    //                }
    //            }
    //            }

    //        }
    //    }
    //}
};
