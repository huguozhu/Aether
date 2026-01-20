export module Aether:D3D12PipelineState;
import :D3D12PipelineState;
import :D3D12Context;
import :RHIStruct;
import :D3D12Definition;
import :Error;


namespace Aether
{
	D3D12PipelineState::D3D12PipelineState(AetherEngine* engine, ERHIPipelineType type)
		:RHIPipelineState(engine, type)
	{

	}

	void* D3D12PipelineState::GetNativePSO()
	{
		return (void*)this->GetD3D12PipelineState();
	}
	ID3D12PipelineState* D3D12PipelineState::GetD3D12PipelineState()
	{
		if (m_pPipelineState)
			return m_pPipelineState.Get();

		AResult ret = A_Success;
		if (ERHIPipelineType::Graphics == m_ePipelineType)
			AETHER_RET_NULL_IF_FAIL(this->CreateGraphicsPipelineState());
		else if (ERHIPipelineType::Compute == m_ePipelineType)
			AETHER_RET_NULL_IF_FAIL(this->CreateComputePipelineState());
		else if (ERHIPipelineType::RayTracing == m_ePipelineType)
			AETHER_RET_NULL_IF_FAIL(this->CreateRayTracingPipelineState());
		else if (ERHIPipelineType::Mesh == m_ePipelineType)
			AETHER_RET_NULL_IF_FAIL(this->CreateMeshPipelineState());
		return m_pPipelineState.Get();
	}

	AResult D3D12PipelineState::CreateGraphicsPipelineState()
	{
        ID3D12Device* pDevice = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance()).GetD3D12Device();
        D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

        // 着色器
        //if (m_GraphicDesc.vertexShader) {
        //    auto shader = static_cast<D3D12Shader*>(m_GraphicDesc.vertexShader);
        //    psoDesc.VS = shader->GetD3D12Bytecode();
        //}

        //if (m_GraphicDesc.pixelShader) {
        //    auto shader = static_cast<D3D12Shader*>(m_GraphicDesc.pixelShader);
        //    psoDesc.PS = shader->GetD3D12Bytecode();
        //}

        //if (m_GraphicDesc.domainShader) {
        //    auto shader = static_cast<D3D12Shader*>(m_GraphicDesc.domainShader);
        //    psoDesc.DS = shader->GetD3D12Bytecode();
        //}

        //if (m_GraphicDesc.hullShader) {
        //    auto shader = static_cast<D3D12Shader*>(m_GraphicDesc.hullShader);
        //    psoDesc.HS = shader->GetD3D12Bytecode();
        //}

        //if (m_GraphicDesc.geometryShader) {
        //    auto shader = static_cast<D3D12Shader*>(m_GraphicDesc.geometryShader);
        //    psoDesc.GS = shader->GetD3D12Bytecode();
        //}

        //// 输入布局
        //std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
        //for (const auto& element : m_GraphicDesc.inputLayout.elements) {
        //    D3D12_INPUT_ELEMENT_DESC inputElement = {};
        //    inputElement.SemanticName = element.semanticName.c_str();
        //    inputElement.SemanticIndex = element.semanticIndex;
        //    inputElement.Format = ConvertFormat(element.format);
        //    inputElement.InputSlot = element.inputSlot;
        //    inputElement.AlignedByteOffset = element.alignedByteOffset;
        //    inputElement.InputSlotClass = element.perInstance
        //        ? D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA
        //        : D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
        //    inputElement.InstanceDataStepRate = element.instanceDataStepRate;
        //    inputElements.push_back(inputElement);
        //}

        //psoDesc.InputLayout.NumElements = static_cast<UINT>(inputElements.size());
        //psoDesc.InputLayout.pInputElementDescs = inputElements.data();

        //// 根签名
        //psoDesc.pRootSignature = m_rootSignature->GetD3D12RootSignature();

        //// 混合状态
        //auto& blendState = m_GraphicDesc.blendState;
        //psoDesc.BlendState.AlphaToCoverageEnable = blendState.alphaToCoverageEnable;
        //psoDesc.BlendState.IndependentBlendEnable = blendState.independentBlendEnable;

        //for (int i = 0; i < 8; i++) {
        //    auto& rtBlend = blendState.renderTarget[i];
        //    psoDesc.BlendState.RenderTarget[i].BlendEnable = rtBlend.blendEnable;
        //    psoDesc.BlendState.RenderTarget[i].SrcBlend = ConvertBlendMode(rtBlend.srcBlend);
        //    psoDesc.BlendState.RenderTarget[i].DestBlend = ConvertBlendMode(rtBlend.destBlend);
        //    psoDesc.BlendState.RenderTarget[i].BlendOp = ConvertBlendOp(rtBlend.blendOp);
        //    psoDesc.BlendState.RenderTarget[i].SrcBlendAlpha = ConvertBlendMode(rtBlend.srcBlendAlpha);
        //    psoDesc.BlendState.RenderTarget[i].DestBlendAlpha = ConvertBlendMode(rtBlend.destBlendAlpha);
        //    psoDesc.BlendState.RenderTarget[i].BlendOpAlpha = ConvertBlendOp(rtBlend.blendOpAlpha);
        //    psoDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = rtBlend.renderTargetWriteMask;
        //}

        //// 光栅化状态
        //auto& rasterizerState = m_GraphicDesc.rasterizerState;
        //psoDesc.RasterizerState.FillMode = ConvertFillMode(rasterizerState.fillMode);
        //psoDesc.RasterizerState.CullMode = ConvertCullMode(rasterizerState.cullMode);
        //psoDesc.RasterizerState.FrontCounterClockwise = rasterizerState.frontCounterClockwise;
        //psoDesc.RasterizerState.DepthBias = rasterizerState.depthBias;
        //psoDesc.RasterizerState.DepthBiasClamp = rasterizerState.depthBiasClamp;
        //psoDesc.RasterizerState.SlopeScaledDepthBias = rasterizerState.slopeScaledDepthBias;
        //psoDesc.RasterizerState.DepthClipEnable = rasterizerState.depthClipEnable;
        //psoDesc.RasterizerState.MultisampleEnable = rasterizerState.multisampleEnable;
        //psoDesc.RasterizerState.AntialiasedLineEnable = rasterizerState.antialiasedLineEnable;
        //psoDesc.RasterizerState.ForcedSampleCount = rasterizerState.forcedSampleCount;
        //psoDesc.RasterizerState.ConservativeRaster = rasterizerState.conservativeRaster
        //    ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON
        //    : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

        //// 深度模板状态
        //auto& depthStencilState = m_GraphicDesc.depthStencilState;
        //psoDesc.DepthStencilState.DepthEnable = depthStencilState.depthEnable;
        //psoDesc.DepthStencilState.DepthWriteMask = depthStencilState.depthWriteMask == DEPTH_WRITE_MASK::ALL
        //    ? D3D12_DEPTH_WRITE_MASK_ALL
        //    : D3D12_DEPTH_WRITE_MASK_ZERO;
        //psoDesc.DepthStencilState.DepthFunc = ConvertComparisonFunc(depthStencilState.depthFunc);
        //psoDesc.DepthStencilState.StencilEnable = depthStencilState.stencilEnable;
        //psoDesc.DepthStencilState.StencilReadMask = depthStencilState.stencilReadMask;
        //psoDesc.DepthStencilState.StencilWriteMask = depthStencilState.stencilWriteMask;
        //psoDesc.DepthStencilState.FrontFace = ConvertStencilOpState(depthStencilState.frontFace);
        //psoDesc.DepthStencilState.BackFace = ConvertStencilOpState(depthStencilState.backFace);

        //// 输出格式
        //psoDesc.NumRenderTargets = m_GraphicDesc.numRenderTargets;
        //for (int i = 0; i < 8; i++) {
        //    psoDesc.RTVFormats[i] = ConvertFormat(m_GraphicDesc.rtvFormats[i]);
        //}
        //psoDesc.DSVFormat = ConvertFormat(m_GraphicDesc.dsvFormat);

        //// 采样
        //psoDesc.SampleDesc.Count = m_GraphicDesc.sampleDesc.count;
        //psoDesc.SampleDesc.Quality = m_GraphicDesc.sampleDesc.quality;
        //psoDesc.SampleMask = m_GraphicDesc.sampleMask;
        //psoDesc.PrimitiveTopologyType = ConvertPrimitiveTopologyType(m_GraphicDesc.primitiveTopology);

        //// 创建PSO
        //HRESULT hr = d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
        //if (FAILED(hr)) {
        //    LOG_ERROR("Failed to create graphics pipeline state: 0x{:X}", hr);
        //    return Err;
        //}

        //// 设置名称
        //if (!m_Desc.name.empty()) {
        //    m_pipelineState->SetName(StringToWString(m_Desc.name).c_str());
        //}

		return A_Success;
	}
	AResult D3D12PipelineState::CreateComputePipelineState()
	{
		return A_Success;
	}
	AResult D3D12PipelineState::CreateRayTracingPipelineState()
	{
		return A_Success;
	}
	AResult D3D12PipelineState::CreateMeshPipelineState()
	{
		return A_Success;
	}

};