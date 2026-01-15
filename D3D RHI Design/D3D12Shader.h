class D3D12Shader : public IShader {
private:
    SHADER_STAGE m_stage;
    std::string m_entryPoint;
    std::vector<uint8_t> m_bytecode;
    ShaderReflection m_reflection;
    
    // D3D12特定
    D3D12_SHADER_BYTECODE m_d3dBytecode;

public:
    D3D12Shader(const ShaderDesc& desc);
    ~D3D12Shader() override;
    
    SHADER_STAGE GetStage() const override { return m_stage; }
    const std::string& GetEntryPoint() const override { return m_entryPoint; }
    const std::vector<uint8_t>& GetBytecode() const override { return m_bytecode; }
    const ShaderReflection& GetReflection() const override { return m_reflection; }
    
    void* GetNativeShader() override { return &m_d3dBytecode; }
    
    D3D12_SHADER_BYTECODE GetD3D12Bytecode() const {
        return m_d3dBytecode;
    }

private:
    bool ReflectShader();
    void ParseInputSignatures(ID3D12ShaderReflection* reflection);
    void ParseConstantBuffers(ID3D12ShaderReflection* reflection);
};

D3D12Shader::D3D12Shader(const ShaderDesc& desc) 
    : m_stage(desc.stage)
    , m_entryPoint(desc.entryPoint)
    , m_bytecode(desc.bytecode) {
    
    // 设置D3D12字节码结构
    m_d3dBytecode.pShaderBytecode = m_bytecode.data();
    m_d3dBytecode.BytecodeLength = m_bytecode.size();
    
    // 反射shader信息
    ReflectShader();
}

bool D3D12Shader::ReflectShader() {
    ComPtr<ID3D12ShaderReflection> reflection;
    
    D3DReflect(m_bytecode.data(), m_bytecode.size(), 
               IID_PPV_ARGS(&reflection));
    
    if (!reflection) {
        return false;
    }
    
    D3D12_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);
    
    // 解析输入签名
    ParseInputSignatures(reflection.Get());
    
    // 解析常量缓冲区
    ParseConstantBuffers(reflection.Get());
    
    // 解析绑定点（描述符表、根常量等）
    for (UINT i = 0; i < shaderDesc.BoundResources; i++) {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        reflection->GetResourceBindingDesc(i, &bindDesc);
        
        // 处理不同类型的资源绑定
        switch (bindDesc.Type) {
        case D3D_SIT_CBUFFER:
            // 处理常量缓冲区绑定
            break;
        case D3D_SIT_TBUFFER:
        case D3D_SIT_TEXTURE:
            // 处理纹理绑定
            break;
        case D3D_SIT_SAMPLER:
            // 处理采样器绑定
            break;
        case D3D_SIT_UAV_RWTYPED:
        case D3D_SIT_UAV_RWSTRUCTURED:
            // 处理UAV绑定
            break;
        }
    }
    
    return true;
}

void D3D12Shader::ParseInputSignatures(ID3D12ShaderReflection* reflection) {
    D3D12_SHADER_DESC shaderDesc;
    reflection->GetDesc(&shaderDesc);
    
    for (UINT i = 0; i < shaderDesc.InputParameters; i++) {
        D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
        reflection->GetInputParameterDesc(i, &paramDesc);
        
        ShaderReflection::InputParameter inputParam;
        inputParam.semanticName = paramDesc.SemanticName;
        inputParam.semanticIndex = paramDesc.SemanticIndex;
        inputParam.slot = paramDesc.Register;
        
        // 确定格式
        if (paramDesc.Mask == 1) {
            if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
                inputParam.format = FORMAT::R32_UINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
                inputParam.format = FORMAT::R32_SINT;
            else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
                inputParam.format = FORMAT::R32_FLOAT;
        }
        // ... 处理其他掩码组合
        
        m_reflection.inputParameters.push_back(inputParam);
    }
}