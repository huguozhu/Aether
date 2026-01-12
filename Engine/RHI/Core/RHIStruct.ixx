export module Aether:RHIStruct;

import <string>;
import <vector>;

export namespace Aether
{
    class RHIShader;

    enum class ECubeFaceType
    {
        Positive_X,
        Negative_X,
        Positive_Y,
        Negative_Y,
        Positive_Z,
        Negative_Z,
        Num,
    };

    enum class ERHIType {
        D3D12,
        Vulkan,
        Metal,
    };

    enum class ECommandListType {
        Graphics,
        Compute,        
        Copy,
        Bundle,
    };

    enum class ECommandListState {
        Recording,
        Executable,
        PendingExecution,
        Invalid,
    };

    enum class EShaderStage {
        Vertex,
        Pixel,
        Geometry,
        Hull,
        Domain,
        Compute,

        Mesh,
        Task,
        Amplification,
        
        RayGen,
        ClosestHit,
        Miss,
        Intersection
    };

    enum class EResourceState {
        Common,
        VertexAndConstantBuffer,
        ShaderResource,
        UnorderedAccess,
        RenderTarget,
        DepthWrite,
        CopyDest,
        CopySource,
        Present,
        AccelerationStructureRead,
        AccelerationStructureWrite,
        ShadingRateSource
    };

    enum class ERHIPipelineType {
        Graphics,
        Compute,
        RayTracing,
        Mesh
    };

    enum class EBlendMode {
        Opaque, AlphaBlend, Additive, PremultipliedAlpha
    };

    enum class EDepthTest {
        Less, LessEqual, Greater, GreaterEqual, Equal, Always, Never
    };

    enum class ECullMode {
        None, Back, Front
    };

    enum class EPrimitiveTopology {
        TriangleList,
        TriangleStrip,
        PointList,
        LineList,
        LineStrip,
        PatchList_1, PatchList_2, PatchList_3, PatchList_32,
    };

    struct RHIBlendState {
        bool bAlphaToCoverage = false;
        bool bIndependentBlend = false;

        struct RenderTargetBlend {
            bool bBlendEnable = false;
            EBlendMode srcColor = EBlendMode::Opaque;
            EBlendMode dstColor = EBlendMode::Opaque;
            EBlendMode srcAlpha = EBlendMode::Opaque;
            EBlendMode dstAlpha = EBlendMode::Opaque;
        };

        RenderTargetBlend targets[8];
    };

    struct RHIDepthStencilState {
        bool bDepthTestEnable = true;
        bool bDepthWriteEnable = true;
        EDepthTest depthFunc = EDepthTest::LessEqual;
        bool bStencilEnable = false;
        uint8_t stencilReadMask = 0xFF;
        uint8_t stencilWriteMask = 0xFF;
    };

    struct RHIRasterizerState {
        ECullMode cullMode = ECullMode::Back;
        bool bScissorEnable = false;
        float depthBias = 0.0f;
        float depthBiasClamp = 0.0f;
        float slopeScaledDepthBias = 0.0f;
        bool bFrontCounterClockwise = false;
    };

    struct RHIMeshPipelineDesc {
        RHIShader* amplificationShader = nullptr;   // Task Shader (可选)
        RHIShader* meshShader = nullptr;            // Mesh Shader (必需)
        RHIShader* pixelShader = nullptr;           // Fragment Shader

        // 输出图元类型（由 Mesh Shader 控制）
        EPrimitiveTopology outputPrimitiveType = EPrimitiveTopology::TriangleList;

        // 渲染目标格式（同 Graphics Pipeline）
        uint32_t rtvFormats[8];
        uint32_t numRenderTargets = 1;
        uint32_t dsvFormat = 0;
        uint32_t sampleCount = 1;

        // 状态
        RHIBlendState blendState;
        RHIRasterizerState rasterizerState;
        RHIDepthStencilState depthStencilState;

        // 根签名
        void* rootSignature = nullptr;

        // 调试名
        std::string debugName;
    };

    enum class ERayTracingShaderGroupType {
        General,            // Raygen, Miss, Callable
        TrianglesHit,       // Hit Group for triangle geometry
        ProceduralHit       // Hit Group for procedural geometry
    };

    struct RHIInputElement {
        std::string semanticName;     // "POSITION", "TEXCOORD", "NORMAL"
        uint32_t semanticIndex = 0;
        uint32_t format;              // DXGI_FORMAT_R32G32B32_FLOAT / VK_FORMAT_R32G32B32_SFLOAT
        uint32_t bufferSlot = 0;
        uint32_t offset = 0;
        bool perInstance = false;
    };

    struct RHIVertexBufferLayout {
        uint32_t stride;
        bool bPerInstance = false;
    };

    struct RHIComputePipelineDesc {
        RHIShader* computeShader = nullptr;

        // 可选：显式指定根签名
        void* rootSignature = nullptr;

        // 调试名称（用于调试工具如 RenderDoc）
        std::string debugName;
    };

    struct RHIGraphicsPipelineDesc {
        // 着色器
        RHIShader* vertexShader = nullptr;
        RHIShader* pixelShader = nullptr;
        RHIShader* geometryShader = nullptr;
        RHIShader* hullShader = nullptr;
        RHIShader* domainShader = nullptr;

        // 输入布局
        std::vector<RHIInputElement> inputElements;
        std::vector<RHIVertexBufferLayout> vertexBufferLayouts;

        // 渲染目标格式
        uint32_t rtvFormats[8];         // e.g., DXGI_FORMAT_R8G8B8A8_UNORM
        uint32_t numRenderTargets = 1;
        uint32_t dsvFormat = 0;        // Depth-stencil format
        uint32_t sampleCount = 1;
        uint32_t sampleQuality = 0;

        // 状态对象
        RHIBlendState blendState;
        RHIRasterizerState rasterizerState;
        RHIDepthStencilState depthStencilState;

        // 其他
        EPrimitiveTopology primitiveTopology = EPrimitiveTopology::TriangleList;
        bool bUseTessellation = false;

        // 根签名（可选，也可由 PSO 外部绑定）
        void* rootSignature = nullptr;
    };


    struct RHIRayTracingShaderGroup {
        ERayTracingShaderGroupType type;
        std::string name;                   // 名称（用于调试）

        // 着色器引用
        RHIShader* raygenShader = nullptr;
        RHIShader* missShader = nullptr;
        RHIShader* closestHitShader = nullptr;
        RHIShader* anyHitShader = nullptr;
        RHIShader* intersectionShader = nullptr;

        // 可选：关联的几何体或材质 ID（高级用法）
        int associatedGeometryID = -1;
    };

    struct RHIRayTracingPipelineDesc 
    {
        std::vector<RHIRayTracingShaderGroup> shaderGroups;
        std::vector<RHIShader*> allShaders;  // 所有着色器（便于编译）

        uint32_t maxRecursionDepth = 1;      // 必须 ≥ 1
        uint32_t maxPayloadSizeInBytes = 32;
        uint32_t maxAttributeSizeInBytes = 32;

        // 全局 & 每 shader 记录的根签名（可选）
        void* globalRootSignature = nullptr;
        void* localRootSignature = nullptr;

        // 调试信息
        std::string debugName;

        // 可选：是否启用光线查询（Ray Query in non-RT shaders）
        bool bAllowRayQueries = false;
    };

    struct CapabilitySet
    {
        bool        TextureSampleCountSupport[16] = { false };
        uint8_t     maxRenderTargetCount = 8;
        //bool        TextureSupport[to_underlying(PixelFormat::Num)][to_underlying(TextureFormatSupportType::Num)] = { {false} };

        //bool        IsTextureSupport(PixelFormat pixel_format, TextureFormatSupportType type) const { return TextureSupport[(uint32_t)pixel_format][(uint32_t)type]; }
    };

};