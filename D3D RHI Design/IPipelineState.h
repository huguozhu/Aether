class IPipelineState {
public:
    virtual ~IPipelineState() = default;
    
    virtual PipelineType GetType() const = 0;
    virtual const PipelineStateDesc& GetDesc() const = 0;
    virtual IRootSignature* GetRootSignature() const = 0;
    
    virtual void* GetNativePSO() = 0;
};

struct PipelineStateDesc {
    PipelineType type = PipelineType::GRAPHICS;
    
    // 图形管线状态
    struct GraphicsDesc {
        IRootSignature* rootSignature = nullptr;
        IShader* vertexShader = nullptr;
        IShader* pixelShader = nullptr;
        IShader* domainShader = nullptr;
        IShader* hullShader = nullptr;
        IShader* geometryShader = nullptr;
        
        // 输入布局
        struct InputLayout {
            struct InputElement {
                std::string semanticName;
                uint32_t semanticIndex;
                FORMAT format;
                uint32_t inputSlot;
                uint32_t alignedByteOffset;
                bool perInstance;
                uint32_t instanceDataStepRate;
            };
            std::vector<InputElement> elements;
        } inputLayout;
        
        // 混合状态
        struct BlendState {
            bool alphaToCoverageEnable = false;
            bool independentBlendEnable = false;
            struct RenderTargetBlendState {
                bool blendEnable = false;
                BLEND_MODE srcBlend = BLEND_MODE::ONE;
                BLEND_MODE destBlend = BLEND_MODE::ZERO;
                BLEND_OP blendOp = BLEND_OP::ADD;
                BLEND_MODE srcBlendAlpha = BLEND_MODE::ONE;
                BLEND_MODE destBlendAlpha = BLEND_MODE::ZERO;
                BLEND_OP blendOpAlpha = BLEND_OP::ADD;
                uint8_t renderTargetWriteMask = 0xF;
            };
            RenderTargetBlendState renderTarget[8];
        } blendState;
        
        // 光栅化状态
        struct RasterizerState {
            FILL_MODE fillMode = FILL_MODE::SOLID;
            CULL_MODE cullMode = CULL_MODE::BACK;
            bool frontCounterClockwise = false;
            int32_t depthBias = 0;
            float depthBiasClamp = 0.0f;
            float slopeScaledDepthBias = 0.0f;
            bool depthClipEnable = true;
            bool multisampleEnable = false;
            bool antialiasedLineEnable = false;
            uint32_t forcedSampleCount = 0;
            bool conservativeRaster = false;
        } rasterizerState;
        
        // 深度模板状态
        struct DepthStencilState {
            bool depthEnable = true;
            DEPTH_WRITE_MASK depthWriteMask = DEPTH_WRITE_MASK::ALL;
            COMPARISON_FUNC depthFunc = COMPARISON_FUNC::LESS;
            bool stencilEnable = false;
            uint8_t stencilReadMask = 0xFF;
            uint8_t stencilWriteMask = 0xFF;
            struct StencilOpState {
                STENCIL_OP stencilFailOp = STENCIL_OP::KEEP;
                STENCIL_OP stencilDepthFailOp = STENCIL_OP::KEEP;
                STENCIL_OP stencilPassOp = STENCIL_OP::KEEP;
                COMPARISON_FUNC stencilFunc = COMPARISON_FUNC::ALWAYS;
            };
            StencilOpState frontFace;
            StencilOpState backFace;
        } depthStencilState;
        
        // 输出格式
        FORMAT rtvFormats[8] = { FORMAT::UNKNOWN };
        FORMAT dsvFormat = FORMAT::UNKNOWN;
        
        // 采样
        SAMPLE_DESC sampleDesc = { 1, 0 };
        uint32_t sampleMask = 0xFFFFFFFF;
        PRIMITIVE_TOPOLOGY primitiveTopology = PRIMITIVE_TOPOLOGY::TRIANGLE_LIST;
        uint32_t numRenderTargets = 1;
    } graphicsDesc;
    
    // 计算管线状态
    struct ComputeDesc {
        IRootSignature* rootSignature = nullptr;
        IShader* computeShader = nullptr;
    } computeDesc;
};