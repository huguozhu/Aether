export module Aether:RHIStruct;

import <string>;
import <vector>;

export namespace Aether
{
/******************************************************************************
* RenderState
*******************************************************************************/
    enum class ECullMode : uint8_t
    {
        None = 0,
        Front,
        Back,
    };

    enum class EFillMode : uint8_t
    {
        Point = 0,
        Wireframe,
        Solid,
    };

    struct RasterizerStateDesc
    {
        bool            bScissorEnable = false;
        bool            bFrontFaceCCW = false;
        bool            bDepthClip = true;
        ECullMode        eCullMode = ECullMode::None;
        EFillMode        eFillMode = EFillMode::Solid;
        float           fLineWidth = 1.0;
    };

    enum class ECompareFunction : uint8_t
    {
        Less = 0,
        LessEqual,
        Greater,
        GreaterEqual,
        Equal,
        NotEqual,
        Never,
        Always,
    };

    enum class EStencilOperation : uint8_t
    {
        Keep = 0,
        Zero,
        Replace,
        Increment,
        Decrement,
        Invert,
        SaturatedIncrement,
        SaturatedDecrement,
    };
    enum class EBlendOperation : uint8_t
    {
        Add = 0,
        Subtract,
        ReverseSubtract,
        Min,
        Max,
    };

    enum class EBlendFactor : uint8_t
    {
        Zero = 0,
        One,

        SrcColor,
        InvSrcColor,
        SrcAlpha,
        InvSrcAlpha,

        DstColor,
        InvDstColor,
        DstAlpha,
        InvDstAlpha,

        SrcAlphaSat,
        BlendFactor,
        InvBlendFactor,

        Src1Color,
        InvSrc1Color,
        Src1Alpha,
        InvSrc1Alpha,
    };

    enum EColorWriteMask : uint8_t
    {
        CWM_Red = 0x01,
        CWM_Green = 0x02,
        CWM_Blue = 0x04,
        CWM_Alpha = 0x08,

        CWM_None = 0x00,
        CWM_RG = CWM_Red | CWM_Green,
        CWM_RGB = CWM_Red | CWM_Green | CWM_Blue,
        CWM_RGBA = CWM_Red | CWM_Green | CWM_Blue | CWM_Alpha,
    };
    /******************************************************************************
    * RHISampler
    *******************************************************************************/
    enum class ETexFilterOp : uint8_t
    {
        Min_Mag_Mip_Point,
        Min_Mag_Point_Mip_Linear,
        Min_Point_Mag_Linear_Mip_Point,
        Min_Point_Mag_Mip_Linear,
        Min_Linear_Mag_Mip_Point,
        Min_Linear_Mag_Point_Mip_Linear,
        Min_Mag_Linear_Mip_Point,
        Min_Mag_Mip_Linear,
        Anisotropic,
    };

    enum class ETexAddressMode : uint8_t
    {
        Wrap,
        Clamp,
        Mirror,
        Border,
    };




    enum class EAlphaMode : uint8_t
    {
        Opaque = 0,
        Mask,
        Blend,
    };

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
        Intersection,
        Num,
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
    enum class EIndexBufferType : uint8_t
    {
        Unknown,
        UInt16,
        UInt32,
    };

    enum class EMeshTopologyType : uint8_t
    {
        Points,
        Lines,
        Line_Strip,
        Triangles,
        Triangle_Strip,

        Unknown = 0xFF,
    };

    enum class EMorphTargetType
    {
        None,
        Position,
        PositionNormal,
    };

    struct MorphInfo
    {
        EMorphTargetType            morph_target_type = EMorphTargetType::None;
        //RHIGpuBufferPtr             render_buffer = nullptr;
        std::vector<float>          morph_target_weights;
        std::vector<float>          prev_morph_target_weights;
        std::vector<std::string>    morph_target_names;
    };

    enum class EDepthTest {
        Less, LessEqual, Greater, GreaterEqual, Equal, Always, Never
    };

    enum class EPrimitiveTopology {
        TriangleList,
        TriangleStrip,
        PointList,
        LineList,
        LineStrip,
        PatchList_1, PatchList_2, PatchList_3, PatchList_32,
    };


    struct RHIMeshPipelineDesc 
    {
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
        //BlendStateDesc blendState;
        //RasterizerStateDesc rasterizerState;
        //RHIDepthStencilStateDesc depthStencilState;

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
        //RHIBlendState blendState;
        //RHIRasterizerState rasterizerState;
        //RHIDepthStencilState depthStencilState;

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