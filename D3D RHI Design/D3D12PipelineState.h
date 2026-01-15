class D3D12PipelineState : public IPipelineState {
private:
    PipelineStateDesc m_desc;
    ComPtr<ID3D12PipelineState> m_pipelineState;
    D3D12RootSignature* m_rootSignature;  // 非拥有，只是引用
    
    D3D12Device* m_device;

public:
    D3D12PipelineState(D3D12Device* device, const PipelineStateDesc& desc);
    ~D3D12PipelineState() override;
    
    PipelineType GetType() const override { return m_desc.type; }
    const PipelineStateDesc& GetDesc() const override { return m_desc; }
    IRootSignature* GetRootSignature() const override { return m_rootSignature; }
    
    void* GetNativePSO() override { return m_pipelineState.Get(); }
    ID3D12PipelineState* GetD3D12PipelineState() { return m_pipelineState.Get(); }

private:
    bool CreateGraphicsPipelineState();
    bool CreateComputePipelineState();
    D3D12_BLEND ConvertBlendMode(BLEND_MODE mode);
    D3D12_BLEND_OP ConvertBlendOp(BLEND_OP op);
    D3D12_FILL_MODE ConvertFillMode(FILL_MODE mode);
    D3D12_CULL_MODE ConvertCullMode(CULL_MODE mode);
    D3D12_COMPARISON_FUNC ConvertComparisonFunc(COMPARISON_FUNC func);
    D3D12_PRIMITIVE_TOPOLOGY_TYPE ConvertPrimitiveTopologyType(PRIMITIVE_TOPOLOGY topology);
};

D3D12PipelineState::D3D12PipelineState(D3D12Device* device, const PipelineStateDesc& desc)
    : m_device(device)
    , m_desc(desc)
    , m_rootSignature(static_cast<D3D12RootSignature*>(desc.graphicsDesc.rootSignature)) {
    
    if (m_desc.type == PipelineType::GRAPHICS) {
        CreateGraphicsPipelineState();
    } else {
        CreateComputePipelineState();
    }
}

bool D3D12PipelineState::CreateGraphicsPipelineState() {
    auto d3dDevice = m_device->GetNativeDevice();
    
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    
    // 着色器
    if (m_desc.graphicsDesc.vertexShader) {
        auto shader = static_cast<D3D12Shader*>(m_desc.graphicsDesc.vertexShader);
        psoDesc.VS = shader->GetD3D12Bytecode();
    }
    
    if (m_desc.graphicsDesc.pixelShader) {
        auto shader = static_cast<D3D12Shader*>(m_desc.graphicsDesc.pixelShader);
        psoDesc.PS = shader->GetD3D12Bytecode();
    }
    
    if (m_desc.graphicsDesc.domainShader) {
        auto shader = static_cast<D3D12Shader*>(m_desc.graphicsDesc.domainShader);
        psoDesc.DS = shader->GetD3D12Bytecode();
    }
    
    if (m_desc.graphicsDesc.hullShader) {
        auto shader = static_cast<D3D12Shader*>(m_desc.graphicsDesc.hullShader);
        psoDesc.HS = shader->GetD3D12Bytecode();
    }
    
    if (m_desc.graphicsDesc.geometryShader) {
        auto shader = static_cast<D3D12Shader*>(m_desc.graphicsDesc.geometryShader);
        psoDesc.GS = shader->GetD3D12Bytecode();
    }
    
    // 输入布局
    std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
    for (const auto& element : m_desc.graphicsDesc.inputLayout.elements) {
        D3D12_INPUT_ELEMENT_DESC inputElement = {};
        inputElement.SemanticName = element.semanticName.c_str();
        inputElement.SemanticIndex = element.semanticIndex;
        inputElement.Format = ConvertFormat(element.format);
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
    
    // 根签名
    psoDesc.pRootSignature = m_rootSignature->GetD3D12RootSignature();
    
    // 混合状态
    auto& blendState = m_desc.graphicsDesc.blendState;
    psoDesc.BlendState.AlphaToCoverageEnable = blendState.alphaToCoverageEnable;
    psoDesc.BlendState.IndependentBlendEnable = blendState.independentBlendEnable;
    
    for (int i = 0; i < 8; i++) {
        auto& rtBlend = blendState.renderTarget[i];
        psoDesc.BlendState.RenderTarget[i].BlendEnable = rtBlend.blendEnable;
        psoDesc.BlendState.RenderTarget[i].SrcBlend = ConvertBlendMode(rtBlend.srcBlend);
        psoDesc.BlendState.RenderTarget[i].DestBlend = ConvertBlendMode(rtBlend.destBlend);
        psoDesc.BlendState.RenderTarget[i].BlendOp = ConvertBlendOp(rtBlend.blendOp);
        psoDesc.BlendState.RenderTarget[i].SrcBlendAlpha = ConvertBlendMode(rtBlend.srcBlendAlpha);
        psoDesc.BlendState.RenderTarget[i].DestBlendAlpha = ConvertBlendMode(rtBlend.destBlendAlpha);
        psoDesc.BlendState.RenderTarget[i].BlendOpAlpha = ConvertBlendOp(rtBlend.blendOpAlpha);
        psoDesc.BlendState.RenderTarget[i].RenderTargetWriteMask = rtBlend.renderTargetWriteMask;
    }
    
    // 光栅化状态
    auto& rasterizerState = m_desc.graphicsDesc.rasterizerState;
    psoDesc.RasterizerState.FillMode = ConvertFillMode(rasterizerState.fillMode);
    psoDesc.RasterizerState.CullMode = ConvertCullMode(rasterizerState.cullMode);
    psoDesc.RasterizerState.FrontCounterClockwise = rasterizerState.frontCounterClockwise;
    psoDesc.RasterizerState.DepthBias = rasterizerState.depthBias;
    psoDesc.RasterizerState.DepthBiasClamp = rasterizerState.depthBiasClamp;
    psoDesc.RasterizerState.SlopeScaledDepthBias = rasterizerState.slopeScaledDepthBias;
    psoDesc.RasterizerState.DepthClipEnable = rasterizerState.depthClipEnable;
    psoDesc.RasterizerState.MultisampleEnable = rasterizerState.multisampleEnable;
    psoDesc.RasterizerState.AntialiasedLineEnable = rasterizerState.antialiasedLineEnable;
    psoDesc.RasterizerState.ForcedSampleCount = rasterizerState.forcedSampleCount;
    psoDesc.RasterizerState.ConservativeRaster = rasterizerState.conservativeRaster 
        ? D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON 
        : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    
    // 深度模板状态
    auto& depthStencilState = m_desc.graphicsDesc.depthStencilState;
    psoDesc.DepthStencilState.DepthEnable = depthStencilState.depthEnable;
    psoDesc.DepthStencilState.DepthWriteMask = depthStencilState.depthWriteMask == DEPTH_WRITE_MASK::ALL 
        ? D3D12_DEPTH_WRITE_MASK_ALL 
        : D3D12_DEPTH_WRITE_MASK_ZERO;
    psoDesc.DepthStencilState.DepthFunc = ConvertComparisonFunc(depthStencilState.depthFunc);
    psoDesc.DepthStencilState.StencilEnable = depthStencilState.stencilEnable;
    psoDesc.DepthStencilState.StencilReadMask = depthStencilState.stencilReadMask;
    psoDesc.DepthStencilState.StencilWriteMask = depthStencilState.stencilWriteMask;
    psoDesc.DepthStencilState.FrontFace = ConvertStencilOpState(depthStencilState.frontFace);
    psoDesc.DepthStencilState.BackFace = ConvertStencilOpState(depthStencilState.backFace);
    
    // 输出格式
    psoDesc.NumRenderTargets = m_desc.graphicsDesc.numRenderTargets;
    for (int i = 0; i < 8; i++) {
        psoDesc.RTVFormats[i] = ConvertFormat(m_desc.graphicsDesc.rtvFormats[i]);
    }
    psoDesc.DSVFormat = ConvertFormat(m_desc.graphicsDesc.dsvFormat);
    
    // 采样
    psoDesc.SampleDesc.Count = m_desc.graphicsDesc.sampleDesc.count;
    psoDesc.SampleDesc.Quality = m_desc.graphicsDesc.sampleDesc.quality;
    psoDesc.SampleMask = m_desc.graphicsDesc.sampleMask;
    psoDesc.PrimitiveTopologyType = ConvertPrimitiveTopologyType(m_desc.graphicsDesc.primitiveTopology);
    
    // 创建PSO
    HRESULT hr = d3dDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
    if (FAILED(hr)) {
        LOG_ERROR("Failed to create graphics pipeline state: 0x{:X}", hr);
        return false;
    }
    
    // 设置名称
    if (!m_desc.name.empty()) {
        m_pipelineState->SetName(StringToWString(m_desc.name).c_str());
    }
    
    return true;
}