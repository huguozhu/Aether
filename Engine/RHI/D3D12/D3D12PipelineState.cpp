module;
#include <windows.h>
#include <d3d12.h>
export module Aether:D3D12PipelineState;
import :D3D12PipelineState;
import :D3D12Context;
import :RHIStruct;
import :D3D12Definition;
import :D3D12Shader;
import :D3D12Translate;
import :D3D12RootSignature;
import :Error;
import :Utils;


#include "Utils/Macros.h"

namespace Aether
{
    D3D12PipelineState::D3D12PipelineState(AetherEngine* engine, RHIGraphicsPipelineDesc desc)
        :RHIPipelineState(engine, desc)
    {
    }
    D3D12PipelineState::D3D12PipelineState(AetherEngine* engine, RHIComputePipelineDesc desc)
        :RHIPipelineState(engine, desc)
    {
    }
    D3D12PipelineState::D3D12PipelineState(AetherEngine* engine, RHIRayTracingPipelineDesc desc)
        :RHIPipelineState(engine, desc)
    {
    }
    D3D12PipelineState::D3D12PipelineState(AetherEngine* engine, RHIMeshShaderPipelineDesc desc)
        :RHIPipelineState(engine, desc)
    {
    }

    void* D3D12PipelineState::GetNativePSO()
	{
        if (ERHIPipelineType::RayTracing == m_ePipelineType)
        {
            if (!m_pStateObject)               
                AETHER_RET_NULL_IF_FAIL(this->CreateRayTracingPipelineState());
            return (void*)m_pStateObject.Get();
        }
        else
        {
            if (!m_pPipelineState)
            {
                if (ERHIPipelineType::Graphics == m_ePipelineType)
                {
                    AETHER_RET_NULL_IF_FAIL(this->CreateGraphicsPipelineState());
                }
                else if (ERHIPipelineType::Compute == m_ePipelineType)
                {
                    AETHER_RET_NULL_IF_FAIL(this->CreateComputePipelineState());
                }
                else if (ERHIPipelineType::Mesh == m_ePipelineType)
                {
                    AETHER_RET_NULL_IF_FAIL(this->CreateMeshPipelineState());
                }
            }
            return (void*)m_pPipelineState.Get();
        }
	}

	AResult D3D12PipelineState::CreateGraphicsPipelineState()
	{
        ID3D12Device* pDevice = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance()).GetD3D12Device();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

        // 根签名
        if (m_GraphicDesc.rootSignature)
            psoDesc.pRootSignature = (ID3D12RootSignature*)m_GraphicDesc.rootSignature->GetNativeRootSignature();

        // 着色器
        if (m_GraphicDesc.vertexShader)
        {
            D3D12Shader* shader = static_cast<D3D12Shader*>(m_GraphicDesc.vertexShader);
            psoDesc.VS = shader->GetD3D12Bytecode();
        }
        if (m_GraphicDesc.pixelShader)
        {
            D3D12Shader* shader = static_cast<D3D12Shader*>(m_GraphicDesc.pixelShader);
            psoDesc.PS = shader->GetD3D12Bytecode();
        }
        if (m_GraphicDesc.domainShader) 
        {
            D3D12Shader* shader = static_cast<D3D12Shader*>(m_GraphicDesc.domainShader);
            psoDesc.DS = shader->GetD3D12Bytecode();
        }
        if (m_GraphicDesc.hullShader)
        {
            D3D12Shader* shader = static_cast<D3D12Shader*>(m_GraphicDesc.hullShader);
            psoDesc.HS = shader->GetD3D12Bytecode();
        }
        if (m_GraphicDesc.geometryShader)
        {
            D3D12Shader* shader = static_cast<D3D12Shader*>(m_GraphicDesc.geometryShader);
            psoDesc.GS = shader->GetD3D12Bytecode();
        }

        // 混合状态
        RHIBlendStateDesc& blendState = m_GraphicDesc.renderState.blend;
        psoDesc.BlendState.AlphaToCoverageEnable = blendState.bAlphaToCoverageEnable;
        psoDesc.BlendState.IndependentBlendEnable = blendState.bIndependentBlendEnable;

        for (int i = 0; i < 8; i++) {
            RHIBlendStateDesc::TargetBlendDesc& rtBlend = blendState.stTargetBlend[i];
            psoDesc.BlendState.RenderTarget[i].BlendEnable = rtBlend.bBlendEnable;

            psoDesc.BlendState.RenderTarget[i].SrcBlend = D3D12Translate::TranslateBlendFactor(rtBlend.eSrcBlendColor);
            psoDesc.BlendState.RenderTarget[i].DestBlend = D3D12Translate::TranslateBlendFactor(rtBlend.eDstBlendColor);
            psoDesc.BlendState.RenderTarget[i].BlendOp = D3D12Translate::TranslateBlendOp(rtBlend.eBlendOpColor);

            psoDesc.BlendState.RenderTarget[i].SrcBlendAlpha = D3D12Translate::TranslateBlendFactor(rtBlend.eSrcBlendAlpha);
            psoDesc.BlendState.RenderTarget[i].DestBlendAlpha = D3D12Translate::TranslateBlendFactor(rtBlend.eDstBlendAlpha);
            psoDesc.BlendState.RenderTarget[i].BlendOpAlpha = D3D12Translate::TranslateBlendOp(rtBlend.eBlendOpAlpha);
            psoDesc.BlendState.RenderTarget[i].RenderTargetWriteMask =
                ((rtBlend.bColorWriteMask & CWM_Red) ? D3D12_COLOR_WRITE_ENABLE_RED : 0)
                | ((rtBlend.bColorWriteMask & CWM_Green) ? D3D12_COLOR_WRITE_ENABLE_GREEN : 0)
                | ((rtBlend.bColorWriteMask & CWM_Blue) ? D3D12_COLOR_WRITE_ENABLE_BLUE : 0)
                | ((rtBlend.bColorWriteMask & CWM_Alpha) ? D3D12_COLOR_WRITE_ENABLE_ALPHA : 0);
        }
        psoDesc.SampleMask = m_GraphicDesc.rtvDesc.sampleMask;

        // 光栅化状态
        RHIRasterizerStateDesc& rasterizerState = m_GraphicDesc.renderState.rasterizer;
        psoDesc.RasterizerState.FillMode = D3D12Translate::TranslateFillMode(rasterizerState.eFillMode);
        psoDesc.RasterizerState.CullMode = D3D12Translate::TranslateCullMode(rasterizerState.eCullMode);
        psoDesc.RasterizerState.FrontCounterClockwise = rasterizerState.bFrontFaceCCW;
        psoDesc.RasterizerState.DepthBias = 0;
        psoDesc.RasterizerState.DepthBiasClamp = 0;
        psoDesc.RasterizerState.SlopeScaledDepthBias = 0;
        psoDesc.RasterizerState.DepthClipEnable = rasterizerState.bDepthClip;
        psoDesc.RasterizerState.MultisampleEnable = false;
        psoDesc.RasterizerState.AntialiasedLineEnable = false;
        psoDesc.RasterizerState.ForcedSampleCount = 0;
        psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        
        // 深度模板状态
        RHIDepthStencilStateDesc& depthStencilState = m_GraphicDesc.renderState.depthStencil;
        psoDesc.DepthStencilState.DepthEnable = depthStencilState.bDepthEnable;
        psoDesc.DepthStencilState.DepthWriteMask = depthStencilState.bDepthWriteMask ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
        psoDesc.DepthStencilState.DepthFunc = D3D12Translate::TranslateCompareFunction(depthStencilState.eDepthFunc);

        psoDesc.DepthStencilState.StencilEnable = depthStencilState.bFrontStencilEnable;
        psoDesc.DepthStencilState.StencilReadMask = depthStencilState.iFrontStencilReadMask;
        psoDesc.DepthStencilState.StencilWriteMask = depthStencilState.iFrontStencilWriteMask;

        psoDesc.DepthStencilState.FrontFace.StencilFailOp       = D3D12Translate::TranslateStencilOp(depthStencilState.eFrontStencilFail);
        psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp  = D3D12Translate::TranslateStencilOp(depthStencilState.eFrontStencilDepthFail);
        psoDesc.DepthStencilState.FrontFace.StencilPassOp       = D3D12Translate::TranslateStencilOp(depthStencilState.eFrontStencilPass);
        psoDesc.DepthStencilState.FrontFace.StencilFunc         = D3D12Translate::TranslateCompareFunction(depthStencilState.eFrontStencilFunction);

        psoDesc.DepthStencilState.BackFace.StencilFailOp        = D3D12Translate::TranslateStencilOp(depthStencilState.eBackStencilFail);
        psoDesc.DepthStencilState.BackFace.StencilDepthFailOp   = D3D12Translate::TranslateStencilOp(depthStencilState.eBackStencilDepthFail);
        psoDesc.DepthStencilState.BackFace.StencilPassOp        = D3D12Translate::TranslateStencilOp(depthStencilState.eBackStencilPass);
        psoDesc.DepthStencilState.BackFace.StencilFunc          = D3D12Translate::TranslateCompareFunction(depthStencilState.eBackStencilFunction);


        // 输入布局
        std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
        for (const RHIInputElement& element : m_GraphicDesc.inputElements)
        {
            D3D12_INPUT_ELEMENT_DESC inputElement = {};
            inputElement.SemanticName = element.semanticName.c_str();
            inputElement.SemanticIndex = element.semanticIndex;
            inputElement.Format = D3D12Translate::TranslateToPlatformFormat(element.format);
            inputElement.InputSlot = element.inputSlot;
            inputElement.AlignedByteOffset = element.alignedByteOffset;
            inputElement.InputSlotClass = element.perInstance
                ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA
                : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            inputElement.InstanceDataStepRate = element.instanceDataStepRate;
            inputElements.push_back(inputElement);
        }
        psoDesc.InputLayout.NumElements = static_cast<UINT>(inputElements.size());
        psoDesc.InputLayout.pInputElementDescs = inputElements.data();
        psoDesc.PrimitiveTopologyType = D3D12Translate::TranslatePrimitiveTopologyType(m_GraphicDesc.primitiveTopology);

        // 输出格式
        psoDesc.NumRenderTargets = m_GraphicDesc.rtvDesc.numRenderTargets;
        for (int i = 0; i < psoDesc.NumRenderTargets; i++)
        {
            psoDesc.RTVFormats[i] = D3D12Translate::TranslateToPlatformFormat(m_GraphicDesc.rtvDesc.rtvFormats[i]);
        }
        psoDesc.DSVFormat = D3D12Translate::TranslateToPlatformFormat(m_GraphicDesc.rtvDesc.dsvFormat);

        // 采样
        psoDesc.SampleDesc.Count = m_GraphicDesc.rtvDesc.sampleCount;
        psoDesc.SampleDesc.Quality = m_GraphicDesc.rtvDesc.sampleQuality;
        
        psoDesc.NodeMask = 0;
        psoDesc.CachedPSO.pCachedBlob = nullptr;
        psoDesc.CachedPSO.CachedBlobSizeInBytes = 0;
        psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
        

        // 创建PSO
        HRESULT hr = pDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));                                                                    
        if (FAILED(hr)) {
            LOG_ERROR("Failed to create graphics pipeline state: 0x{:X}", hr);
            return ERR_SYSTEM_ERROR;
        }

        // 设置名称
        if (!m_GraphicDesc.debugName.empty())
            m_pPipelineState->SetName(StringToWString(m_GraphicDesc.debugName.c_str()).c_str());        

		return A_Success;
	}
	AResult D3D12PipelineState::CreateComputePipelineState()
	{
        ID3D12Device* pDevice = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance()).GetD3D12Device();
        D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
        // 根签名
        psoDesc.pRootSignature = (ID3D12RootSignature*)m_ComputeDesc.rootSignature->GetNativeRootSignature();

        // 着色器
        if (m_ComputeDesc.computeShader)
        {
            D3D12Shader* shader = static_cast<D3D12Shader*>(m_ComputeDesc.computeShader);
            psoDesc.CS = shader->GetD3D12Bytecode();
        }
        psoDesc.NodeMask = 0;
        psoDesc.CachedPSO.pCachedBlob = nullptr;
        psoDesc.CachedPSO.CachedBlobSizeInBytes = 0;
        psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
        HRESULT hr = pDevice->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
        if (FAILED(hr)) {
            LOG_ERROR("Failed to create compute pipeline state: 0x{:X}", hr);
            return ERR_SYSTEM_ERROR;
        }
		return A_Success;
	}

    AResult D3D12PipelineState::RayTracing_AddDxilLibrarySubobject(RHIShader* shader)
    {
        D3D12Shader* d3dShader = (D3D12Shader*)shader;
        // 创建DXIL库描述
        D3D12_DXIL_LIBRARY_DESC* libraryDesc = new D3D12_DXIL_LIBRARY_DESC();
        m_vSubobjectData.push_back(libraryDesc);

        libraryDesc->DXILLibrary.pShaderBytecode = d3dShader->GetD3D12Bytecode().pShaderBytecode;
        libraryDesc->DXILLibrary.BytecodeLength = d3dShader->GetD3D12Bytecode().BytecodeLength;

        // 创建导出描述
        D3D12_EXPORT_DESC* exportDesc = new D3D12_EXPORT_DESC();
        m_vSubobjectData.push_back(exportDesc);

        exportDesc->Name = nullptr;// StringToWString(shader.exportName).c_str();
        exportDesc->ExportToRename = nullptr;
        exportDesc->Flags = D3D12_EXPORT_FLAG_NONE;

        libraryDesc->NumExports = 1;
        libraryDesc->pExports = exportDesc;

        // 创建子对象
        D3D12_STATE_SUBOBJECT subobject = {};
        subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_DXIL_LIBRARY;
        subobject.pDesc = libraryDesc;
        m_vSubobjects.push_back(subobject);
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_AddHitGroupSubobject(const HitGroup& hitGroup)
    {
        D3D12_HIT_GROUP_DESC* hitGroupDesc = new D3D12_HIT_GROUP_DESC();
        m_vSubobjectData.push_back(hitGroupDesc);

        hitGroupDesc->HitGroupExport = (LPCWSTR)StringToWString(hitGroup.name.c_str()).c_str();

        if (!hitGroup.closestHitShader.empty()) {
            hitGroupDesc->ClosestHitShaderImport = StringToWString(hitGroup.closestHitShader.c_str()).c_str();
        }

        if (!hitGroup.anyHitShader.empty()) {
            hitGroupDesc->AnyHitShaderImport = StringToWString(hitGroup.anyHitShader.c_str()).c_str();
        }

        if (!hitGroup.intersectionShader.empty()) {
            hitGroupDesc->IntersectionShaderImport = StringToWString(hitGroup.intersectionShader.c_str()).c_str();
        }

        hitGroupDesc->Type = D3D12Translate::TranslateHitGroupType(hitGroup.type);

        // 创建子对象
        D3D12_STATE_SUBOBJECT subobject = {};
        subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_HIT_GROUP;
        subobject.pDesc = hitGroupDesc;
        m_vSubobjects.push_back(subobject);
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_AddRootSignatureSubobject(RHIRootSignature* rootSig, bool isLocal)
    {
        D3D12RootSignature* d3dRootSig = static_cast<D3D12RootSignature*>(rootSig);

        D3D12_GLOBAL_ROOT_SIGNATURE* rootSigDesc = new D3D12_GLOBAL_ROOT_SIGNATURE();
        m_vSubobjectData.push_back(rootSigDesc);

        rootSigDesc->pGlobalRootSignature = (ID3D12RootSignature*)d3dRootSig->GetNativeRootSignature();

        // 创建子对象
        D3D12_STATE_SUBOBJECT subobject = {};
        subobject.Type = isLocal ? D3D12_STATE_SUBOBJECT_TYPE_LOCAL_ROOT_SIGNATURE
            : D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
        subobject.pDesc = rootSigDesc;
        m_vSubobjects.push_back(subobject);
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_AddShaderConfigSubobject()
    {
        D3D12_RAYTRACING_SHADER_CONFIG* shaderConfig = new D3D12_RAYTRACING_SHADER_CONFIG();
        m_vSubobjectData.push_back(shaderConfig);

        shaderConfig->MaxPayloadSizeInBytes = m_RayTracingDesc.maxPayloadSize;
        shaderConfig->MaxAttributeSizeInBytes = m_RayTracingDesc.maxAttributeSize;

        D3D12_STATE_SUBOBJECT subobject = {};
        subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
        subobject.pDesc = shaderConfig;
        m_vSubobjects.push_back(subobject);
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_AddPipelineConfigSubobject()
    {
        D3D12_RAYTRACING_PIPELINE_CONFIG* pipelineConfig = new D3D12_RAYTRACING_PIPELINE_CONFIG();
        m_vSubobjectData.push_back(pipelineConfig);

        pipelineConfig->MaxTraceRecursionDepth = m_RayTracingDesc.maxRecursionDepth;

        D3D12_STATE_SUBOBJECT subobject = {};
        subobject.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
        subobject.pDesc = pipelineConfig;
        m_vSubobjects.push_back(subobject);
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_AddShaderAssociationSubobject(const ShaderAssociation& association)
    {
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_CreateSubobjects()
    {
        // 为每个着色器添加DXIL库子对象
        for (RHIShader* shader : m_RayTracingDesc.shaders) {
            RayTracing_AddDxilLibrarySubobject(shader);
        }

        // 添加命中组子对象
        for (const HitGroup& hitGroup : m_RayTracingDesc.hitGroups) {
            RayTracing_AddHitGroupSubobject(hitGroup);
        }

        // 添加全局根签名子对象
        if (m_RayTracingDesc.globalRootSignature) {
            RayTracing_AddRootSignatureSubobject(m_RayTracingDesc.globalRootSignature, false);
        }

        // 添加局部根签名子对象
        for (const auto& [exportName, rootSig] : m_RayTracingDesc.localRootSignatures) {
            RayTracing_AddRootSignatureSubobject(rootSig, true);
        }

        // 添加着色器配置子对象
        RayTracing_AddShaderConfigSubobject();

        // 添加管线配置子对象
        RayTracing_AddPipelineConfigSubobject();

        // 添加着色器关联子对象
        for (const auto& [exportName, association] : m_RayTracingDesc.shaderAssociations) {
            RayTracing_AddShaderAssociationSubobject(association);
        }
        return A_Success;
    }
    AResult D3D12PipelineState::RayTracing_CollectShaderIdentifiers()
    {
        if (!m_pStateObjectProperties)
            return ERR_SYSTEM_ERROR;
        

        // 收集所有着色器标识符
        for (RHIShader* shader : m_RayTracingDesc.shaders)
        {
            void* identifier = m_pStateObjectProperties->GetShaderIdentifier(
                StringToWString(shader->GetEntryPoint().c_str()).c_str());
            if (identifier)
            {
                ShaderIdentifier shaderId;
                memcpy(shaderId.data.data(), identifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
                m_vShaderIdentifiers[shader->GetEntryPoint().c_str()] = shaderId;
            }
        }
        // 收集所有命中组标识符
        for (const auto& hitGroup : m_RayTracingDesc.hitGroups)
        {
            void* identifier = m_pStateObjectProperties->GetShaderIdentifier(
                StringToWString(hitGroup.name.c_str()).c_str());
            if (identifier) {
                ShaderIdentifier hitGroupId;
                memcpy(hitGroupId.data.data(), identifier, D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES);
                m_vShaderIdentifiers[hitGroup.name] = hitGroupId;
            }
        }
        return A_Success;
    }
	AResult D3D12PipelineState::CreateRayTracingPipelineState()
	{
        ID3D12Device5* pDevice5 = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance()).GetD3D12Device5();
        if (!pDevice5)
        {
            LOG_ERROR("Ray Tracing not support");
            return ERR_NOT_SUPPORT;
        }

        // 创建子对象集合
        AETHER_RETIF_FAIL(this->RayTracing_CreateSubobjects());
        
        // 创建状态对象描述
        D3D12_STATE_OBJECT_DESC stateObjectDesc = {};
        stateObjectDesc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
        stateObjectDesc.NumSubobjects = static_cast<UINT>(m_vSubobjects.size());
        stateObjectDesc.pSubobjects = m_vSubobjects.data();

        // 创建状态对象
        HRESULT hr = pDevice5->CreateStateObject(&stateObjectDesc, IID_PPV_ARGS(&m_pStateObject));
        if (FAILED(hr)) {
            LOG_ERROR("Failed to create ray tracing state object: 0x{:X}", hr);
            return ERR_SYSTEM_ERROR;
        }

        // 获取状态对象属性
        hr = m_pStateObject.As(&m_pStateObjectProperties);
        if (FAILED(hr)) {
            LOG_ERROR("Failed to get state object properties: 0x{:X}", hr);
            return ERR_NOT_SUPPORT;
        }

        // 设置调试名称
        if (!m_RayTracingDesc.debugName.empty())
            m_pStateObject->SetName(StringToWString(m_RayTracingDesc.debugName.c_str()).c_str());

        RayTracing_CollectShaderIdentifiers();

		return A_Success;
	}

	AResult D3D12PipelineState::CreateMeshPipelineState()
	{
        ID3D12Device* pDevice = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance()).GetD3D12Device();
		return A_Success;
	}

};