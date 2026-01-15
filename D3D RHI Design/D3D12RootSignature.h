class D3D12RootSignature : public IRootSignature {
private:
    RootSignatureDesc m_desc;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    
    D3D12Device* m_device;
    std::vector<D3D12_ROOT_PARAMETER> m_rootParameters;
    std::vector<D3D12_DESCRIPTOR_RANGE> m_descriptorRanges;
    std::vector<D3D12_STATIC_SAMPLER_DESC> m_staticSamplers;

public:
    D3D12RootSignature(D3D12Device* device, const RootSignatureDesc& desc);
    ~D3D12RootSignature() override;
    
    uint32_t GetParameterCount() const override { return static_cast<uint32_t>(m_rootParameters.size()); }
    const RootSignatureDesc& GetDesc() const override { return m_desc; }
    
    void* GetNativeRootSignature() override { return m_rootSignature.Get(); }
    ID3D12RootSignature* GetD3D12RootSignature() { return m_rootSignature.Get(); }

private:
    bool CreateRootSignature();
    D3D12_ROOT_PARAMETER_TYPE ConvertParameterType(ROOT_PARAMETER_TYPE type);
    D3D12_SHADER_VISIBILITY ConvertShaderVisibility(SHADER_VISIBILITY visibility);
    D3D12_DESCRIPTOR_RANGE_TYPE ConvertDescriptorRangeType(DESCRIPTOR_RANGE_TYPE type);
    D3D12_FILTER ConvertFilter(FILTER_MODE filter);
    D3D12_TEXTURE_ADDRESS_MODE ConvertAddressMode(TEXTURE_ADDRESS_MODE mode);
};

D3D12RootSignature::D3D12RootSignature(D3D12Device* device, const RootSignatureDesc& desc)
    : m_device(device)
    , m_desc(desc) {
    
    CreateRootSignature();
}

bool D3D12RootSignature::CreateRootSignature() {
    auto d3dDevice = m_device->GetNativeDevice();
    
    // 转换根参数
    for (const auto& param : m_desc.parameters) {
        D3D12_ROOT_PARAMETER rootParam = {};
        rootParam.ParameterType = ConvertParameterType(param.type);
        rootParam.ShaderVisibility = ConvertShaderVisibility(param.visibility);
        
        switch (param.type) {
        case ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE: {
            std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
            for (const auto& range : param.descriptorTable.ranges) {
                D3D12_DESCRIPTOR_RANGE d3dRange = {};
                d3dRange.RangeType = ConvertDescriptorRangeType(range.type);
                d3dRange.NumDescriptors = range.numDescriptors;
                d3dRange.BaseShaderRegister = range.baseShaderRegister;
                d3dRange.RegisterSpace = range.registerSpace;
                d3dRange.OffsetInDescriptorsFromTableStart = range.offsetInDescriptorsFromTableStart;
                ranges.push_back(d3dRange);
                
                // 保存到成员变量中以便生命周期管理
                m_descriptorRanges.push_back(d3dRange);
            }
            
            rootParam.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(ranges.size());
            rootParam.DescriptorTable.pDescriptorRanges = ranges.data();
            break;
        }
        
        case ROOT_PARAMETER_TYPE_32BIT_CONSTANTS: {
            rootParam.Constants.Num32BitValues = param.constants.num32BitValues;
            rootParam.Constants.ShaderRegister = param.constants.shaderRegister;
            rootParam.Constants.RegisterSpace = param.constants.registerSpace;
            break;
        }
        
        case ROOT_PARAMETER_TYPE_CBV:
        case ROOT_PARAMETER_TYPE_SRV:
        case ROOT_PARAMETER_TYPE_UAV: {
            rootParam.Descriptor.ShaderRegister = param.descriptor.shaderRegister;
            rootParam.Descriptor.RegisterSpace = param.descriptor.registerSpace;
            break;
        }
        }
        
        m_rootParameters.push_back(rootParam);
    }
    
    // 转换静态采样器
    for (const auto& sampler : m_desc.staticSamplers) {
        D3D12_STATIC_SAMPLER_DESC staticSampler = {};
        staticSampler.Filter = ConvertFilter(sampler.filter);
        staticSampler.AddressU = ConvertAddressMode(sampler.addressU);
        staticSampler.AddressV = ConvertAddressMode(sampler.addressV);
        staticSampler.AddressW = ConvertAddressMode(sampler.addressW);
        staticSampler.MipLODBias = sampler.mipLODBias;
        staticSampler.MaxAnisotropy = sampler.maxAnisotropy;
        staticSampler.ComparisonFunc = ConvertComparisonFunc(sampler.comparisonFunc);
        staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
        staticSampler.MinLOD = sampler.minLOD;
        staticSampler.MaxLOD = sampler.maxLOD;
        staticSampler.ShaderRegister = sampler.shaderRegister;
        staticSampler.RegisterSpace = sampler.registerSpace;
        staticSampler.ShaderVisibility = ConvertShaderVisibility(sampler.visibility);
        
        m_staticSamplers.push_back(staticSampler);
    }
    
    // 创建根签名描述
    D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
    rootSignatureDesc.NumParameters = static_cast<UINT>(m_rootParameters.size());
    rootSignatureDesc.pParameters = m_rootParameters.data();
    rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(m_staticSamplers.size());
    rootSignatureDesc.pStaticSamplers = m_staticSamplers.data();
    rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    
    // 序列化根签名
    ComPtr<ID3DBlob> signatureBlob;
    ComPtr<ID3DBlob> errorBlob;
    
    HRESULT hr = D3D12SerializeRootSignature(
        &rootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        &signatureBlob,
        &errorBlob
    );
    
    if (FAILED(hr)) {
        if (errorBlob) {
            LOG_ERROR("Failed to serialize root signature: {}", 
                     static_cast<const char*>(errorBlob->GetBufferPointer()));
        }
        return false;
    }
    
    // 创建根签名
    hr = d3dDevice->CreateRootSignature(
        0,
        signatureBlob->GetBufferPointer(),
        signatureBlob->GetBufferSize(),
        IID_PPV_ARGS(&m_rootSignature)
    );
    
    if (FAILED(hr)) {
        LOG_ERROR("Failed to create root signature: 0x{:X}", hr);
        return false;
    }
    
    // 设置名称
    if (!m_desc.name.empty()) {
        m_rootSignature->SetName(StringToWString(m_desc.name).c_str());
    }
    
    return true;
}