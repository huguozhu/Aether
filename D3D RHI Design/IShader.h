class IShader {
public:
    virtual ~IShader() = default;
    
    virtual SHADER_STAGE GetStage() const = 0;
    virtual const std::string& GetEntryPoint() const = 0;
    virtual const std::vector<uint8_t>& GetBytecode() const = 0;
    virtual const ShaderReflection& GetReflection() const = 0;
    
    virtual void* GetNativeShader() = 0;
};

struct ShaderDesc {
    SHADER_STAGE stage;
    std::string entryPoint;
    std::vector<uint8_t> bytecode;
    std::string source;  // 可选，用于动态编译
};

struct ShaderReflection {
    struct InputParameter {
        std::string semanticName;
        uint32_t semanticIndex;
        FORMAT format;
        uint32_t slot;
    };
    
    struct ConstantBuffer {
        std::string name;
        uint32_t size;
        uint32_t bindPoint;
        uint32_t bindCount;
    };
    
    std::vector<InputParameter> inputParameters;
    std::vector<ConstantBuffer> constantBuffers;
    // ... 其他反射信息
};