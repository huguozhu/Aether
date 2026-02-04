export module Aether:RHIRootSignature;
import :Engine;
import :EngineDefinition;
import :Hash;
import <string>;


export namespace Aether
{
     /******************************************************************************
      * Root Signature
      *******************************************************************************/
    // 根参数类型
    enum class ERootParameterType : uint32_t
    {
        DescriptorTable,        // 描述符表
        Constant32Bit,          // 32位常量
        ConstantBuffer,         // 常量缓冲区视图 (CBV)
        ShaderResourceView,     // 着色器资源视图 (SRV)
        UnorderedAccessView,    // 无序访问视图 (UAV)
    };

    // 着色器可见性
    enum class EShaderVisibility : uint32_t
    {        
        Vertex,             // 仅顶点着色器
        Pixel,              // 仅像素着色器
        Geometry,           // 仅几何着色器
        Domain,             // 仅域着色器
        Hull,               // 仅外壳着色器
        
        Compute,           // 仅计算着色器
        Mesh,               // 仅网格着色器
        Amplification,      // 仅放大着色器 (Mesh Shader)        
        All,                // 所有着色器阶段
    };

    // 描述符范围类型
    enum class EDescriptorRangeType : uint32_t
    {
        Srv,                // 着色器资源视图
        Uav,                // 无序访问视图
        Cbv,                // 常量缓冲区视图
        Sampler,            // 采样器
    };

    // 描述符范围
    struct DescriptorRange
    {
        EDescriptorRangeType type;                     // 范围类型
        uint32_t numDescriptors;                        // 描述符数量
        uint32_t baseShaderRegister;                    // 基础着色器寄存器
        uint32_t registerSpace;                         // 寄存器空间
        uint32_t offsetInDescriptorsFromTableStart;     // 从表开始的偏移量

        //DescriptorRange()
        //    : type(EDescriptorRangeType::Srv)
        //    , numDescriptors(1)
        //    , baseShaderRegister(0)
        //    , registerSpace(0)
        //    , offsetInDescriptorsFromTableStart(-1) {
        //}
    };


    // 描述符表
    struct DescriptorTable 
    {
        std::vector<DescriptorRange> ranges;           // 范围数组

        DescriptorTable() = default;
        explicit DescriptorTable(const std::vector<DescriptorRange>& ranges)
            : ranges(ranges) {}
    };

    // 常量参数
    struct Constants
    {
        uint32_t num32BitValues;      // 32位值的数量
        uint32_t shaderRegister;      // 着色器寄存器
        uint32_t registerSpace;       // 寄存器空间

        Constants()
            : num32BitValues(0), shaderRegister(0), registerSpace(0) {}
        Constants(uint32_t numValues, uint32_t reg, uint32_t space = 0)
            : num32BitValues(numValues), shaderRegister(reg), registerSpace(space) {}
    };

    // 描述符参数
    struct Descriptor 
    {
        uint32_t shaderRegister;      // 着色器寄存器
        uint32_t registerSpace;       // 寄存器空间
        Descriptor() : shaderRegister(0), registerSpace(0) {}
        Descriptor(uint32_t reg, uint32_t space = 0) : shaderRegister(reg), registerSpace(space) {}
    };


    // 根参数
    struct RootParameter 
    {
        ERootParameterType type;                // 参数类型
        EShaderVisibility visibility;           // 可见性
         
        struct
        {
            DescriptorTable descriptorTable;    // 描述符表
            Constants constants;                // 32位常量
            Descriptor descriptor;              // 单个描述符 (CBV/SRV/UAV)
        };
        std::string name;                       // 调试名称

        // 构造函数
        RootParameter()
            : type(ERootParameterType::DescriptorTable), visibility(EShaderVisibility::All), name("") 
        {
        }
    };

    /******************************************************************************
     * Sampler
     *******************************************************************************/
     // 纹理地址模式
    enum class ETextureAddressMode
    {
        WRAP,           // 环绕
        MIRROR,         // 镜像
        CLAMP,          // 夹取
        BORDER,         // 边界
        MIRROR_ONCE     // 镜像一次
    };

    // 过滤器模式
    enum class EFilterMode
    {
        MIN_MAG_MIP_POINT,                  // 点过滤
        MIN_MAG_POINT_MIP_LINEAR,           // 点/点/线性
        MIN_POINT_MAG_LINEAR_MIP_POINT,     // 点/线性/点
        MIN_POINT_MAG_MIP_LINEAR,           // 点/线性/线性
        MIN_LINEAR_MAG_MIP_POINT,           // 线性/点/点

        MIN_LINEAR_MAG_POINT_MIP_LINEAR,    // 线性/点/线性
        MIN_MAG_LINEAR_MIP_POINT,           // 线性/线性/点
        MIN_MAG_MIP_LINEAR,                 // 线性过滤
        ANISOTROPIC,                        // 各向异性
        COMPARISON_MIN_MAG_MIP_POINT,       // 比较点过滤

        COMPARISON_MIN_MAG_MIP_LINEAR,      // 比较线性过滤
        COMPARISON_ANISOTROPIC,             // 比较各向异性
        MINIMUM_MIN_MAG_MIP_POINT,          // 最小值点过滤
        MINIMUM_MIN_MAG_MIP_LINEAR,         // 最小值线性过滤
        MINIMUM_ANISOTROPIC,                // 最小值各向异性

        MAXIMUM_MIN_MAG_MIP_POINT,          // 最大值点过滤
        MAXIMUM_MIN_MAG_MIP_LINEAR,         // 最大值线性过滤
        MAXIMUM_ANISOTROPIC                 // 最大值各向异性
    };

    // 静态采样器描述
    struct RHISamplerDesc
    {
        EFilterMode filter;                     // 过滤模式
        ETextureAddressMode addressU;          // U方向寻址模式
        ETextureAddressMode addressV;          // V方向寻址模式
        ETextureAddressMode addressW;          // W方向寻址模式
        float mipLODBias;                       // MIP LOD偏移
        uint32_t maxAnisotropy;                 // 最大各向异性
        ECompareFunction comparisonFunc;         // 比较函数
        float borderColor[4];                   // 边界颜色
        float minLOD;                           // 最小LOD
        float maxLOD;                           // 最大LOD
        uint32_t shaderRegister;                // 着色器寄存器
        uint32_t registerSpace;                 // 寄存器空间
        EShaderVisibility visibility;           // 可见性

        std::string name;                       // 调试名称

        RHISamplerDesc()
            : filter(EFilterMode::MIN_MAG_MIP_LINEAR)
            , addressU(ETextureAddressMode::WRAP)
            , addressV(ETextureAddressMode::WRAP)
            , addressW(ETextureAddressMode::WRAP)
            , mipLODBias(0.0f)
            , maxAnisotropy(1)
            , comparisonFunc(ECompareFunction::Never)
            , minLOD(0.0f)
            , maxLOD(10000.0f)
            , shaderRegister(0)
            , registerSpace(0)
            , visibility(EShaderVisibility::All)
            , name("")
        {
            borderColor[0] = borderColor[1] = borderColor[2] = borderColor[3] = 0.0f;
        }

        static RHISamplerDesc GetSamplerDescByName(std::string const& name);       
        static RHISamplerDesc PointSampler();
        static RHISamplerDesc LinearSampler();
        static RHISamplerDesc BilinearSampler();
        static RHISamplerDesc PrefilterMapSampler();
        static RHISamplerDesc AnisotropicSampler();
        static RHISamplerDesc SkyboxSampler();
        static RHISamplerDesc ShadowMapSampler();
        static RHISamplerDesc NoiseSampler();

    };

    // 根签名标志
    enum class ERootSignatureFlags
    {
        NONE = 0,
        ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT = 0x1,   // 允许输入汇编器输入布局
        DENY_VERTEX_SHADER_ROOT_ACCESS = 0x2,       // 拒绝顶点着色器根访问
        DENY_HULL_SHADER_ROOT_ACCESS = 0x4,         // 拒绝外壳着色器根访问
        DENY_DOMAIN_SHADER_ROOT_ACCESS = 0x8,       // 拒绝域着色器根访问
        DENY_GEOMETRY_SHADER_ROOT_ACCESS = 0x10,    // 拒绝几何着色器根访问
        DENY_PIXEL_SHADER_ROOT_ACCESS = 0x20,       // 拒绝像素着色器根访问
        DENY_AMPLIFICATION_SHADER_ROOT_ACCESS = 0x40,  // 拒绝放大着色器根访问
        DENY_MESH_SHADER_ROOT_ACCESS = 0x80,        // 拒绝网格着色器根访问
        ALLOW_STREAM_OUTPUT = 0x100,                // 允许流输出
        LOCAL_ROOT_SIGNATURE = 0x200,               // 本地根签名（光线追踪）
        DENY_COMPUTE_SHADER_ROOT_ACCESS = 0x400,    // 拒绝计算着色器根访问
        CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED = 0x800,  // CBV/SRV/UAV堆直接索引
        SAMPLER_HEAP_DIRECTLY_INDEXED = 0x1000      // 采样器堆直接索引
    };
    // 根签名版本
    enum class ERootSignatureVersion
    {
        Version_1_0,    // D3D12 1.0
        Version_1_1,    // D3D12 1.1 - 添加了本地根签名支持
        Version_1_2     // D3D12 1.2 - 添加了静态采样器改进
    };

    // 根签名描述符
    struct RHIRootSignatureDesc
    {
        std::vector<RootParameter> parameters;          // 根参数数组
        std::vector<RHISamplerDesc> staticSamplers;     // 静态采样器数组
        ERootSignatureFlags flags;                      // 根签名标志
        ERootSignatureVersion version;                  // 根签名版本

        std::string debugName;                          // 调试名称

        // 构造函数
        RHIRootSignatureDesc()
            : flags(ERootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)
            , version(ERootSignatureVersion::Version_1_0), debugName("") {
        }

        RHIRootSignatureDesc(
            const std::vector<RootParameter>& params,
            const std::vector<RHISamplerDesc>& samplers = {},
            ERootSignatureFlags flags = ERootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
            const std::string& name = "")
            : parameters(params), staticSamplers(samplers), flags(flags), debugName(name) {
        }
    };

    class RHIRootSignature 
    {
    public:
        RHIRootSignature(AetherEngine* engine, RHIRootSignatureDesc desc)
            :m_pEngine(engine), m_Desc(desc) {}
        virtual ~RHIRootSignature() = default;

        virtual uint32_t GetParameterCount() const = 0;
        virtual const RHIRootSignatureDesc& GetDesc() const { return m_Desc; }

        virtual void* GetNativeRootSignature() = 0;
        size_t  Hash()
        {
            if (m_Hash == 0)
            {
                char* begin = (char*)(&m_Desc);
                char* end = begin + sizeof(m_Desc);
                m_Hash = HashRange(begin, end);
            }
            return m_Hash;
        }

    protected:
        AetherEngine* m_pEngine = nullptr;
        RHIRootSignatureDesc m_Desc;
        size_t  m_Hash = 0;
    };
};