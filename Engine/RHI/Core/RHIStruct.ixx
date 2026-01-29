export module Aether:RHIStruct;

import :Vector;
import :Format;
import :EngineDefinition;
import :Hash;
import <string>;
import <vector>;

#define STRUCT_HASH() \
    char* begin = (char*)(this); \
    char* end = begin + sizeof(*this); \
    return HashRange(begin, end); \

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

    struct RHIRasterizerStateDesc
    {
        bool            bScissorEnable = false;
        bool            bFrontFaceCCW = false;
        bool            bDepthClip = true;
        ECullMode       eCullMode = ECullMode::None;
        EFillMode       eFillMode = EFillMode::Solid;
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
        None,
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
        DepthRead,
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
    enum class EIndexBufferType : uint32_t
    {
        Unknown,
        UInt16,
        UInt32,
    };

    enum class EMeshTopologyType : uint32_t
    {
        Points,
        Lines,
        Line_Strip,
        Triangles,
        Triangle_Strip,
        PatchList_1, 
        PatchList_2, 
        PatchList_3, 
        PatchList_32,
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
        //RHIBufferPtr             render_buffer = nullptr;
        std::vector<float>          morph_target_weights;
        std::vector<float>          prev_morph_target_weights;
        std::vector<std::string>    morph_target_names;
    };

    enum class ERayTracingShaderGroupType
    {
        General,            // Raygen, Miss, Callable
        TrianglesHit,       // Hit Group for triangle geometry
        ProceduralHit       // Hit Group for procedural geometry
    };

    struct RHIInputElement
    {
        std::string semanticName;     // "POSITION", "TEXCOORD", "NORMAL"
        uint32_t semanticIndex = 0;
        PixelFormat format;
        uint32_t inputSlot = 0;
        uint32_t alignedByteOffset = 0;
        uint32_t offset = 0;
        uint32_t instanceDataStepRate = 0;
        bool perInstance = false;
    };

    struct RHIVertexBufferLayout 
    {
        uint32_t stride;
        bool bPerInstance = false;
    };

    struct RHIComputePipelineDesc 
    {
        RHIShader* computeShader = nullptr;

        // 根签名（可选，也可由 PSO 外部绑定）
        RHIRootSignature* rootSignature = nullptr;

        // 调试名称（用于调试工具如 RenderDoc）
        std::string debugName;
        size_t GetHash() { STRUCT_HASH(); }
    };

    struct RHIDepthStencilStateDesc
    {
        bool                    bDepthEnable = true;
        bool                    bDepthWriteMask = true;
        ECompareFunction        eDepthFunc = ECompareFunction::Less;

        bool                    bFrontStencilEnable = false;
        ECompareFunction        eFrontStencilFunction = ECompareFunction::Always;
        EStencilOperation       eFrontStencilFail = EStencilOperation::Keep;
        EStencilOperation       eFrontStencilDepthFail = EStencilOperation::Keep;
        EStencilOperation       eFrontStencilPass = EStencilOperation::Keep;
        uint16_t                iFrontStencilRef = 0;
        uint16_t                iFrontStencilReadMask = 0xffff;
        uint16_t                iFrontStencilWriteMask = 0xffff;

        bool                    bBackStencilEnable = false;
        ECompareFunction        eBackStencilFunction = ECompareFunction::Always;
        EStencilOperation       eBackStencilFail = EStencilOperation::Keep;
        EStencilOperation       eBackStencilDepthFail = EStencilOperation::Keep;
        EStencilOperation       eBackStencilPass = EStencilOperation::Keep;
        uint16_t                iBackStencilRef = 0;
        uint16_t                iBackStencilReadMask = 0xffff;
        uint16_t                iBackStencilWriteMask = 0xffff;
    };

    struct RHIBlendStateDesc
    {
        float4          fBlendFactor = float4(1.0);
        uint32_t        iSampleMask = 0xffffffff;

        bool            bIndependentBlendEnable = false;
        bool            bAlphaToCoverageEnable = false;

        struct TargetBlendDesc
        {
            bool            bBlendEnable = false;
            bool            bLogicOpEnable = false;
            EBlendFactor    eSrcBlendColor = EBlendFactor::One;
            EBlendFactor    eDstBlendColor = EBlendFactor::Zero;
            EBlendOperation eBlendOpColor = EBlendOperation::Add;

            EBlendFactor    eSrcBlendAlpha = EBlendFactor::One;
            EBlendFactor    eDstBlendAlpha = EBlendFactor::Zero;
            EBlendOperation eBlendOpAlpha = EBlendOperation::Add;
            EColorWriteMask bColorWriteMask = CWM_RGBA;
        };
        std::array<TargetBlendDesc, 8> stTargetBlend;
    };

    struct RHIRenderStateDesc
    {
        RHIRasterizerStateDesc     rasterizer = {};
        RHIDepthStencilStateDesc   depthStencil = {};
        RHIBlendStateDesc          blend = {};

        static const RHIRenderStateDesc& Default3D();
        static const RHIRenderStateDesc& Default2D();
        static const RHIRenderStateDesc& PostProcess();
        static const RHIRenderStateDesc& PostProcessAccumulate();
        static const RHIRenderStateDesc& Skybox();
        static const RHIRenderStateDesc& Particle();
        static const RHIRenderStateDesc& WaterMark();

        static const RHIRenderStateDesc& GBuffer();
        static const RHIRenderStateDesc& ShadowCopyR();
        static const RHIRenderStateDesc& ShadowCopyG();
        static const RHIRenderStateDesc& ShadowCopyB();
        static const RHIRenderStateDesc& ShadowCopyA();
        static const RHIRenderStateDesc& Lighting();
        static const RHIRenderStateDesc& DepthDisable();

        size_t   Hash() const;
        bool     operator==(RHIRenderStateDesc const& rhs) const;
        bool     operator<(RHIRenderStateDesc const& rhs) const;
    };

    struct RHIGraphicsPipelineDesc 
    {
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
        PixelFormat rtvFormats[8];         // e.g., DXGI_FORMAT_R8G8B8A8_UNORM
        uint32_t numRenderTargets = 1;
        PixelFormat dsvFormat;        // Depth-stencil format
        uint32_t sampleCount = 1;
        uint32_t sampleQuality = 0;
        uint32_t sampleMask = 0xffffffff;

        // 状态对象
        RHIRenderStateDesc renderState;

        // 其他
        EMeshTopologyType primitiveTopology = EMeshTopologyType::Triangles;

        // 根签名（可选，也可由 PSO 外部绑定）
        RHIRootSignature* rootSignature = nullptr;
        std::string name;
        size_t GetHash() { STRUCT_HASH(); }
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

    // 命中组
    enum class EHitGroupType
    {
        Triangles,
        Procedural_Primitive,
    };
    enum class ERaytracingPipelineFlags
    {
        None,
    };
    // 着色器标识符（用于光线追踪）
    struct ShaderIdentifier 
    {
        std::vector<uint8_t> data;  // D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES = 32

        ShaderIdentifier() : data(32, 0) {}

        bool IsValid() const {
            return std::any_of(data.begin(), data.end(), [](uint8_t b) { return b != 0; });
        }

        void Clear() { std::fill(data.begin(), data.end(), 0); }
    };
    // 着色器关联配置
    struct ShaderAssociation 
    {
        std::string exportName;          // 导出着色器名称
        std::vector<std::string> subobjects;  // 关联的子对象

        ShaderAssociation() = default;
        explicit ShaderAssociation(const std::string& name) : exportName(name) {}
    };
    struct HitGroup 
    {
        std::string name;                // 命中组名称
        std::string closestHitShader;    // 最近命中着色器名称
        std::string anyHitShader;        // 任意命中着色器名称
        std::string intersectionShader;  // 相交着色器名称
        EHitGroupType type;             // 命中组类型

        HitGroup(): type(EHitGroupType::Triangles) {}
    };

    struct RHIRayTracingPipelineDesc 
    {

        uint32_t maxPayloadSize;         // 最大负载大小（字节）
        uint32_t maxAttributeSize;       // 最大属性大小（字节）
        uint32_t maxRecursionDepth;       // 最大递归深度

        // 着色器配置
        std::vector<RHIShader*> shaders;
        std::vector<HitGroup> hitGroups;

        // 全局根签名
        RHIRootSignature* globalRootSignature;

        // 局部根签名关联
        std::unordered_map<std::string, RHIRootSignature*> localRootSignatures;

        // 着色器关联
        std::unordered_map<std::string, ShaderAssociation> shaderAssociations;

        // 管线配置标志
        ERaytracingPipelineFlags flags;

        // 调试名称
        std::string name;

        RHIRayTracingPipelineDesc()
            : maxPayloadSize(sizeof(float) * 8)  // 默认：2个float4
            , maxAttributeSize(sizeof(float) * 8) // 默认：重心坐标 + 实例索引
            , maxRecursionDepth(1)
            , globalRootSignature(nullptr)
            , flags(ERaytracingPipelineFlags::None)
        {}
        size_t GetHash() { STRUCT_HASH(); }
    };

    struct RHIMeshShaderPipelineDesc
    {
        RHIShader* amplificationShader = nullptr;   // Task Shader (可选)
        RHIShader* meshShader = nullptr;            // Mesh Shader (必需)
        RHIShader* pixelShader = nullptr;           // Fragment Shader

        // 输出图元类型（由 Mesh Shader 控制）
        EMeshTopologyType outputPrimitiveType = EMeshTopologyType::Triangles;

        // 渲染目标格式（同 Graphics Pipeline）
        uint32_t rtvFormats[8];
        uint32_t numRenderTargets = 1;
        uint32_t dsvFormat = 0;
        uint32_t sampleCount = 1;

        // 状态
        RHIRenderStateDesc renderState;

        // 根签名
        void* rootSignature = nullptr;

        // 调试名
        std::string name;

        // 网格着色器特定配置
        uint32_t maxPayloadSize = 32;         // 最大负载大小
        uint32_t maxAttributeSize = 32;       // 最大属性大小
        uint32_t maxShaderGroupSize = 128;    // 最大着色器组大小

        size_t GetHash() { STRUCT_HASH(); }
    };

    struct CapabilitySet
    {
        bool        TextureSampleCountSupport[16] = { false };
        uint8_t     maxRenderTargetCount = 8;
        //bool        TextureSupport[to_underlying(PixelFormat::Num)][to_underlying(TextureFormatSupportType::Num)] = { {false} };
        //bool        IsTextureSupport(PixelFormat pixel_format, TextureFormatSupportType type) const { return TextureSupport[(uint32_t)pixel_format][(uint32_t)type]; }
    };


    enum class EVertexElementUsage : uint8_t
    {
        Unknown,
        Position,
        TexCoord,
        Normal,
        Color,
        BlendWeight,
        BlendIndex,
        Tangent,
        Binormal,
        Instance,
    };

    enum class IndexBufferType : uint8_t
    {
        Unknown,
        UInt16,
        UInt32,
    };

    struct VertexStreamLayout
    {
        uint32_t                buffer_offset = 0;
        uint32_t                usage_index = 0;
        uint32_t                instance_divisor = 1;
        VertexFormat            format = VertexFormat::Unknown;
        EVertexElementUsage     usage = EVertexElementUsage::Position;
        uint8_t                 is_instance_attrib = false;
        uint8_t                 pad;
    };

    struct VertexStream
    {
        RHIBufferPtr              render_buffer = nullptr;
        uint32_t                        offset = 0;
        uint32_t                        stride = 0;
        std::vector<VertexStreamLayout> layouts;
        uint8_t                         is_instance_stream = false;
    };
    

};