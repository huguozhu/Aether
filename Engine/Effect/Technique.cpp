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


#include "Utils/Macros.h"
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

        TechniquePtr tech = MakeSharedPtr<Technique>(m_pEngine, this);
        tech->SetName(activedTechName);

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

        tech->SetRenderStateDesc(renderStateDesc);

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
        return nullptr;
    }

    Technique* VirtualTechnique::Concrete(const std::vector<EffectPredefine>& predefines)
    {
        return Concrete(predefines, m_defaultRenderState);
    }

    Technique* VirtualTechnique::Concrete()
    {
        std::vector<EffectPredefine> null_predefines;
        return Concrete(null_predefines, m_defaultRenderState);
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
        case EffectDataType::SampledTexture:
        {
            // do nothing
            break;
        }
        default:
            break;
        }
    }

    AResult Technique::Build()
    {
        // collect all params in shaders
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
                param.fallbackName = resource.fallback_name;
                param.bindings[stage] = resource.binding;
                if (param.dataType == EffectDataType::SampledTexture)
                {
                    param.textureParamName = resource.texture_name;
                    param.samplerParamName = resource.sampler_name;
                }
                CreateEffectVariable(param);

                m_params[param.name] = std::move(param);
            }

            // another pass to check if SampledTexture has related separate texture&sampler
            //for (auto& resource : shaderRes->reflectInfo.resources)
            //{
            //    if (resource.type == ResourceType::SampledTexture)
            //    {
            //        auto& sampledTextureParam = m_params[resource.name]; // always find

            //        auto samplerIt = m_params.find(sampledTextureParam.samplerParamName);
            //        if (samplerIt == m_params.end())
            //        {
            //            LOG_ERROR("has no sampler param %s in combined SampledTexture %s", resource.sampler_name.c_str(), resource.name.c_str());
            //            // do something?
            //        }

            //        auto textureIt = m_params.find(sampledTextureParam.textureParamName);
            //        if (textureIt == m_params.end())
            //        {
            //            LOG_ERROR("has no texture param %s in combined SampledTexture %s", resource.texture_name.c_str(), resource.name.c_str());
            //            // do something?
            //        }
            //    }
            //}
        }

        RHIPipelineStatePtr pPipeline = nullptr;
        // ÅÐ¶ÏÊÇÄÄÖÖPipeLine : Graphics/Compute/RayTracing/Mesh
        if (m_shaderRes[(uint32_t)EShaderStage::Vertex])
        {
            RHIGraphicsPipelineDesc desc;
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
            desc.renderState = m_RenderStateDesc;
            pPipeline = m_pEngine->RHIContextInstance().CreateGraphicPipelineState(desc);
        }
        else if (m_shaderRes[(uint32_t)EShaderStage::Compute])
        {
            RHIComputePipelineDesc desc;
            RHIShader* shader = m_pEngine->EffectInstance().CreateShader(EShaderStage::Compute, m_shaderRes[(uint32_t)EShaderStage::Compute]);
            if (shader)
            {
                desc.computeShader = shader;
                desc.debugName = m_techName;
            }
            pPipeline = m_pEngine->RHIContextInstance().CreateComputePipelineState(desc);
        }
        else if (m_shaderRes[(uint32_t)EShaderStage::Mesh])
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
        }
        else if (m_shaderRes[(uint32_t)EShaderStage::RayGen])
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
        }        
       
        return A_Success;
    }

    //AResult Technique::Render(RHIMeshPtr const& mesh)
    //{
    //    RHIContext& rc = m_pContext->RHIContextInstance();
    //    RHIRenderStatePtr state;
    //    if (m_pContext->EnableTransparent() && mesh->GetMaterial() && mesh->GetMaterial()->alpha_mode != AlphaMode::Opaque)
    //    {
    //        if (!m_RenderStateForTransparent)
    //        {
    //            RenderStateDesc rs = m_RenderStateDesc;
    //            if (!mesh->GetMaterial()->double_sided && rs.rasterizer.eCullMode == CullMode::None)
    //                rs.rasterizer.eCullMode = CullMode::Back;
    //            rs.rasterizer.bFrontFaceCCW = true;
    //            rs.depthStencil.bDepthWriteMask = false;
    //            rs.blend.stTargetBlend[0].bBlendEnable = true;
    //            rs.blend.stTargetBlend[0].eSrcBlendColor = BlendFactor::SrcAlpha;
    //            rs.blend.stTargetBlend[0].eDstBlendColor = BlendFactor::InvSrcAlpha;
    //            rs.blend.stTargetBlend[0].eBlendOpColor = BlendOperation::Add;
    //            rs.blend.stTargetBlend[0].eSrcBlendAlpha = BlendFactor::One;
    //            rs.blend.stTargetBlend[0].eDstBlendAlpha = BlendFactor::One;
    //            rs.blend.stTargetBlend[0].eBlendOpAlpha = BlendOperation::Add;
    //            m_RenderStateForTransparent = rc.GetRenderState(rs);
    //        }
    //        state = m_RenderStateForTransparent;
    //    }
    //    else
    //    {
    //        if (!m_RenderState)
    //        {
    //            m_RenderState = rc.GetRenderState(m_RenderStateDesc);
    //        }
    //        state = m_RenderState;
    //    }

    //    if (!mesh->GetRenderState())
    //    {
    //        RenderStateDesc rsd = state->GetRenderStateDesc();
    //        if (mesh->GetMaterial() && !mesh->GetMaterial()->double_sided)
    //            rsd.rasterizer.eCullMode = CullMode::None;
    //        else
    //            rsd.rasterizer.eCullMode = CullMode::None;
    //        rsd.rasterizer.bFrontFaceCCW = mesh->m_bFrontFaceCCW;
    //        state = rc.GetRenderState(rsd);

    //        mesh->SetRenderState(state);
    //    }

    //    if (m_pContext->GetRHIType() == RHIType::GLES && !m_bOpenGLAlreadyRemapBinding)
    //    {
    //        // OpenGL need to remap the binding point, it need to be unique in the whole program
    //        uint32_t uboBinding = 0;
    //        //uint32_t ssboBinding = 0;
    //        uint32_t samplerBinding = 0;
    //        for (auto& paramPair : m_params)
    //        {
    //            auto& param = paramPair.second;
    //            for (size_t stage = 0; stage != SHADER_STAGE_COUNT; stage++)
    //            {
    //                if (param.bindings[stage] == INVALID_BINDING_POINT)
    //                    continue;
    //                switch (param.dataType)
    //                {
    //                case EffectDataType::ConstantBuffer:
    //                {
    //                    param.bindings[stage] = uboBinding++;
    //                    break;
    //                }
    //                //case EffectDataType::Buffer:
    //                //{
    //                //    param.bindings[stage] = ssboBinding++;
    //                //    break;
    //                //}
    //                //case EffectDataType::RWBuffer:
    //                //{
    //                //    param.bindings[stage] = ssboBinding++;
    //                //    break;
    //                //}
    //                case EffectDataType::SampledTexture:
    //                {
    //                    param.bindings[stage] = samplerBinding++;
    //                    break;
    //                }
    //                default:
    //                    break;
    //                }
    //            }
    //        }
    //        m_bOpenGLAlreadyRemapBinding = true;
    //    }

    //    rc.BindRHIProgram(m_pProgram.get());
    //    Commit();
    //    AResult ret = rc.Render(m_pProgram.get(), mesh);
    //    Uncommit();
    //    return ret;
    //}

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
    //AResult Technique::Commit()
    //{
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
    //                RHIBufferPtr rb;
    //                param.variable->Value(rb);
    //                if (rb)
    //                    rc.BindConstantBuffer((ShaderType)stage, param.bindings[stage], rb.get(), param.fallbackName.c_str());
    //                else
    //                    LOG_WARNING("param %s has no resource binding", param.name.c_str());
    //                break;
    //            }
    //            case EffectDataType::Buffer:
    //            {
    //                RHIShaderResourceViewPtr v;
    //                param.variable->Value(v);
    //                if (v)
    //                    rc.BindRHISrv((ShaderType)stage, param.bindings[stage], v.get(), param.name.c_str());
    //                else
    //                    LOG_WARNING("param %s has no resource binding", param.name.c_str());
    //                break;
    //            }
    //            case EffectDataType::RWBuffer:
    //            {
    //                RHIUnorderedAccessViewPtr v;
    //                param.variable->Value(v);
    //                if (v)
    //                    rc.BindRHIUav((ShaderType)stage, param.bindings[stage], v.get(), param.name.c_str());
    //                else
    //                    LOG_WARNING("param %s has no resource binding", param.name.c_str());
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
    //                        RHITexturePtr tex;
    //                        param.variable->Value(tex);
    //                        if (tex)
    //                            rc.BindTexture((ShaderType)stage, param.bindings[stage], tex.get(), param.name.c_str());
    //                        else
    //                            LOG_WARNING("param %s has no resource binding", param.name.c_str());
    //                        break;
    //                    }
    //                    case EffectDataType::RWTexture:
    //                    {
    //                        RHITexturePtr tex;
    //                        param.variable->Value(tex);
    //                        if (tex)
    //                            rc.BindRWTexture((ShaderType)stage, param.bindings[stage], tex.get(), param.name.c_str());
    //                        else
    //                            LOG_WARNING("param %s has no resource binding", param.name.c_str());
    //                        break;
    //                    }
    //                    case EffectDataType::Sampler:
    //                    {
    //                        RHISamplerPtr sampler;
    //                        param.variable->Value(sampler);
    //                        if (!sampler)
    //                        {
    //                            sampler = rc.GetSampler(SamplerDesc::GetSamplerDescByName(param.name));
    //                            *(param.variable) = sampler;
    //                        }
    //                        rc.BindSampler((ShaderType)stage, param.bindings[stage], sampler.get(), param.name.c_str());
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
    //                        auto texIt = m_params.find(param.textureParamName);
    //                        if (texIt != m_params.end())
    //                        {
    //                            RHITexturePtr tex = nullptr;
    //                            texIt->second.variable->Value(tex);
    //                            rc.BindTexture((ShaderType)stage, param.bindings[stage], tex.get(), param.name.c_str());
    //                        }
    //                    }
    //                }
    //            }
    //            }
    //        }
    //    }
    //    return A_Success;
    //}

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

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // TechniqueInstance
    //AResult TechniqueInstance::Build()
    //{
    //    for (auto& formalParam : m_pTech->GetFormalParams())
    //    {
    //        ActualParam actualParam;
    //        std::unique_ptr<EffectVariable> var;
    //        switch (formalParam.second.dataType)
    //        {
    //            case EffectDataType::ConstantBuffer:
    //            case EffectDataType::Buffer:
    //            case EffectDataType::RWBuffer:
    //            {
    //                actualParam.variable = MakeUniquePtr<EffectVariableRenderBuffer>();
    //                break;
    //            }
    //            case EffectDataType::Texture:
    //            case EffectDataType::RWTexture:
    //            {
    //                actualParam.variable = MakeUniquePtr<EffectVariableTexture>();
    //                break;
    //            }
    //            case EffectDataType::Sampler:
    //            {
    //                actualParam.variable = MakeUniquePtr<EffectVariableSampler>();
    //                break;
    //            }
    //            case EffectDataType::SampledTexture:
    //            {
    //                // do nothing
    //                break;
    //            }
    //            default:
    //                break;
    //        }
    //    }
    //
    //    return A_Success;
    //}

};
