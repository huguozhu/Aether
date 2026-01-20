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
        Sampler,                // 采样器
    };

    // 着色器可见性
    enum class EShaderVisibility : uint32_t
    {
        All,                // 所有着色器阶段
        Vertex,             // 仅顶点着色器
        Pixel,              // 仅像素着色器
        Domain,             // 仅域着色器
        Hull,               // 仅外壳着色器
        Geometry,           // 仅几何着色器
        Computer,           // 仅计算着色器
        Amplification,      // 仅放大着色器 (Mesh Shader)
        Mesh                // 仅网格着色器
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

        // 描述符表构造
        static RootParameter CreateDescriptorTable(const std::vector<DescriptorRange>& ranges,
            EShaderVisibility visibility = EShaderVisibility::All, const std::string& name = "") 
        {
            RootParameter param;
            param.type = ERootParameterType::DescriptorTable;
            param.visibility = visibility;
            param.descriptorTable.ranges = ranges;
            param.name = name;
            return param;
        }

        // 32位常量构造
        static RootParameter CreateConstants(uint32_t num32BitValues, uint32_t shaderRegister,
            uint32_t registerSpace = 0, EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") 
        {
            RootParameter param;
            param.type = ERootParameterType::Constant32Bit;
            param.visibility = visibility;
            param.constants = Constants(num32BitValues, shaderRegister, registerSpace);
            param.name = name;
            return param;
        }

        // CBV构造
        static RootParameter CreateCBV(uint32_t shaderRegister, uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All, const std::string& name = "") 
        {
            RootParameter param;
            param.type = ERootParameterType::ConstantBuffer;
            param.visibility = visibility;
            param.descriptor = Descriptor(shaderRegister, registerSpace);
            param.name = name;
            return param;
        }

        // SRV构造
        static RootParameter CreateSRV(uint32_t shaderRegister, uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All, const std::string& name = "") 
        {
            RootParameter param;
            param.type = ERootParameterType::ShaderResourceView;
            param.visibility = visibility;
            param.descriptor = Descriptor(shaderRegister, registerSpace);
            param.name = name;
            return param;
        }

        // UAV构造
        static RootParameter CreateUAV(uint32_t shaderRegister, uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All, const std::string& name = "") 
        {
            RootParameter param;
            param.type = ERootParameterType::UnorderedAccessView;
            param.visibility = visibility;
            param.descriptor = Descriptor(shaderRegister, registerSpace);
            param.name = name;
            return param;
        }

        // 采样器构造
        static RootParameter CreateSampler(uint32_t shaderRegister, uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,const std::string& name = "") 
        {
            RootParameter param;
            param.type = ERootParameterType::Sampler;
            param.visibility = visibility;
            param.descriptor = Descriptor(shaderRegister, registerSpace);
            param.name = name;
            return param;
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
    struct StaticSamplerDesc
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

        StaticSamplerDesc()
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
        // 线性环绕采样器
        static StaticSamplerDesc CreateLinearWrap(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "LinearWrap") {

            StaticSamplerDesc desc;
            desc.filter = EFilterMode::MIN_MAG_MIP_LINEAR;
            desc.addressU = ETextureAddressMode::WRAP;
            desc.addressV = ETextureAddressMode::WRAP;
            desc.addressW = ETextureAddressMode::WRAP;
            desc.shaderRegister = shaderRegister;
            desc.registerSpace = registerSpace;
            desc.visibility = visibility;
            desc.name = name;
            return desc;
        }

        // 点夹取采样器
        static StaticSamplerDesc CreatePointClamp(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "PointClamp") {

            StaticSamplerDesc desc;
            desc.filter = EFilterMode::MIN_MAG_MIP_POINT;
            desc.addressU = ETextureAddressMode::CLAMP;
            desc.addressV = ETextureAddressMode::CLAMP;
            desc.addressW = ETextureAddressMode::CLAMP;
            desc.shaderRegister = shaderRegister;
            desc.registerSpace = registerSpace;
            desc.visibility = visibility;
            desc.name = name;
            return desc;
        }

        // 线性夹取采样器
        static StaticSamplerDesc CreateLinearClamp(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "LinearClamp") {

            StaticSamplerDesc desc;
            desc.filter = EFilterMode::MIN_MAG_MIP_LINEAR;
            desc.addressU = ETextureAddressMode::CLAMP;
            desc.addressV = ETextureAddressMode::CLAMP;
            desc.addressW = ETextureAddressMode::CLAMP;
            desc.shaderRegister = shaderRegister;
            desc.registerSpace = registerSpace;
            desc.visibility = visibility;
            desc.name = name;
            return desc;
        }

        // 各向异性环绕采样器
        static StaticSamplerDesc CreateAnisotropicWrap(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            uint32_t maxAnisotropy = 8,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "AnisotropicWrap") {

            StaticSamplerDesc desc;
            desc.filter = EFilterMode::ANISOTROPIC;
            desc.addressU = ETextureAddressMode::WRAP;
            desc.addressV = ETextureAddressMode::WRAP;
            desc.addressW = ETextureAddressMode::WRAP;
            desc.maxAnisotropy = maxAnisotropy;
            desc.shaderRegister = shaderRegister;
            desc.registerSpace = registerSpace;
            desc.visibility = visibility;
            desc.name = name;
            return desc;
        }

        // 阴影比较采样器
        static StaticSamplerDesc CreateShadowSampler(uint32_t shaderRegister, uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::Pixel, const std::string& name = "ShadowSampler")
        {
            StaticSamplerDesc desc;
            desc.filter = EFilterMode::COMPARISON_MIN_MAG_MIP_LINEAR;
            desc.addressU = ETextureAddressMode::BORDER;
            desc.addressV = ETextureAddressMode::BORDER;
            desc.addressW = ETextureAddressMode::BORDER;
            desc.comparisonFunc = ECompareFunction::LessEqual;
            desc.borderColor[0] = desc.borderColor[1] = desc.borderColor[2] = desc.borderColor[3] = 1.0f;
            desc.shaderRegister = shaderRegister;
            desc.registerSpace = registerSpace;
            desc.visibility = visibility;
            desc.name = name;
            return desc;
        }
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
        std::vector<StaticSamplerDesc> staticSamplers;  // 静态采样器数组
        ERootSignatureFlags flags;                     // 根签名标志
        ERootSignatureVersion version;                 // 根签名版本

        std::string name;                               // 调试名称

        // 构造函数
        RHIRootSignatureDesc()
            : flags(ERootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT)
            , version(ERootSignatureVersion::Version_1_0), name("") {
        }

        RHIRootSignatureDesc(
            const std::vector<RootParameter>& params,
            const std::vector<StaticSamplerDesc>& samplers = {},
            ERootSignatureFlags flags = ERootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT,
            const std::string& name = "")
            : parameters(params), staticSamplers(samplers), flags(flags), name(name) {
        }

        // 常用根签名预设

        // 简单的图形根签名：CBV0 + 纹理表 + 采样器表
        static RHIRootSignatureDesc CreateGraphicsBasic(const std::string& name = "GraphicsBasic")
        {
            std::vector<RootParameter> params;

            // CBV: b0
            params.push_back(RootParameter::CreateCBV(0, 0, EShaderVisibility::All, "CameraBuffer"));

            // SRV描述符表: t0-t15
            std::vector<DescriptorRange> srvRanges;
            srvRanges.push_back(DescriptorRange{
                EDescriptorRangeType::Srv,
                16,  // t0-t15
                0,
                0,
                0xffffffff//D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
                });
            params.push_back(RootParameter::CreateDescriptorTable(
                srvRanges, EShaderVisibility::Pixel, "TextureTable"));

            // 采样器描述符表: s0-s7
            std::vector<DescriptorRange> samplerRanges;
            samplerRanges.push_back(DescriptorRange{
                EDescriptorRangeType::Sampler,
                8,  // s0-s7
                0,
                0,
                0xffffffff//D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
                });
            params.push_back(RootParameter::CreateDescriptorTable(
                samplerRanges, EShaderVisibility::Pixel, "SamplerTable"));

            std::vector<StaticSamplerDesc> staticSamplers;
            staticSamplers.push_back(StaticSamplerDesc::CreateLinearWrap(0));
            staticSamplers.push_back(StaticSamplerDesc::CreatePointClamp(1));

            return RHIRootSignatureDesc(params, staticSamplers,
                ERootSignatureFlags::ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, name);
        }

        // 计算着色器根签名：常量 + UAV表 + SRV表
        static RHIRootSignatureDesc CreateComputeBasic(const std::string& name = "ComputeBasic")
        {
            std::vector<RootParameter> params;
            // 常量: 16个32位值
            params.push_back(RootParameter::CreateConstants(16, 0, 0,
                EShaderVisibility::Computer, "ComputeConstants"));

            // UAV描述符表: u0-u7
            std::vector<DescriptorRange> uavRanges;
            uavRanges.push_back(DescriptorRange{
                EDescriptorRangeType::Uav,
                8,  // u0-u7
                0,
                0,
                0xffffffff //D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
                });
            params.push_back(RootParameter::CreateDescriptorTable(
                uavRanges, EShaderVisibility::Computer, "UAVTable"));

            // SRV描述符表: t0-t7
            std::vector<DescriptorRange> srvRanges;
            srvRanges.push_back(DescriptorRange{
                EDescriptorRangeType::Srv,
                8,  // t0-t7
                0,
                0,
                0xffffffff //D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND
                });
            params.push_back(RootParameter::CreateDescriptorTable(
                srvRanges, EShaderVisibility::Computer, "SRVTable"));

            return RHIRootSignatureDesc(params, {},
                ERootSignatureFlags::NONE, name);
        }
    };


    class RootSignatureBuilder 
    {
    public:
        RHIRootSignatureDesc m_desc;

    public:
        RootSignatureBuilder() = default;
        RootSignatureBuilder(const std::string& name)
        {
            m_desc.name = name;
        }

        // 添加参数
        RootSignatureBuilder& AddParameter(const RootParameter& param) 
        {
            m_desc.parameters.push_back(param);
            return *this;
        }

        // 添加描述符表
        RootSignatureBuilder& AddDescriptorTable(
            const std::vector<DescriptorRange>& ranges,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") 
        {
            m_desc.parameters.push_back(RootParameter::CreateDescriptorTable(ranges, visibility, name));
            return *this;
        }

        // 添加32位常量
        RootSignatureBuilder& AddConstants(
            uint32_t num32BitValues,
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") {

            m_desc.parameters.push_back(
                RootParameter::CreateConstants(num32BitValues, shaderRegister, registerSpace, visibility, name));
            return *this;
        }

        // 添加CBV
        RootSignatureBuilder& AddCBV(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") {

            m_desc.parameters.push_back(
                RootParameter::CreateCBV(shaderRegister, registerSpace, visibility, name));
            return *this;
        }

        // 添加SRV
        RootSignatureBuilder& AddSRV(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") {

            m_desc.parameters.push_back(
                RootParameter::CreateSRV(shaderRegister, registerSpace, visibility, name));
            return *this;
        }

        // 添加UAV
        RootSignatureBuilder& AddUAV(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") {

            m_desc.parameters.push_back(
                RootParameter::CreateUAV(shaderRegister, registerSpace, visibility, name));
            return *this;
        }

        // 添加采样器
        RootSignatureBuilder& AddSampler(
            uint32_t shaderRegister,
            uint32_t registerSpace = 0,
            EShaderVisibility visibility = EShaderVisibility::All,
            const std::string& name = "") {

            m_desc.parameters.push_back(
                RootParameter::CreateSampler(shaderRegister, registerSpace, visibility, name));
            return *this;
        }

        // 添加静态采样器
        RootSignatureBuilder& AddStaticSampler(const StaticSamplerDesc& sampler) {
            m_desc.staticSamplers.push_back(sampler);
            return *this;
        }

        // 设置标志
        RootSignatureBuilder& SetFlags(ERootSignatureFlags flags) {
            m_desc.flags = flags;
            return *this;
        }

        // 设置版本
        RootSignatureBuilder& SetVersion(ERootSignatureVersion version) {
            m_desc.version = version;
            return *this;
        }

        // 设置名称
        RootSignatureBuilder& SetName(const std::string& name) {
            m_desc.name = name;
            return *this;
        }

        // 构建根签名描述
        RHIRootSignatureDesc Build() { return m_desc; }

        // 重置构建器
        void Reset() { m_desc = RHIRootSignatureDesc(); }

        // 预设构建
        static RHIRootSignatureDesc BuildGraphicsBasic() {
            return RHIRootSignatureDesc::CreateGraphicsBasic();
        }

        static RHIRootSignatureDesc BuildComputeBasic() {
            return RHIRootSignatureDesc::CreateComputeBasic();
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