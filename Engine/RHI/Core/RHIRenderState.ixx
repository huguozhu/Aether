export module Aether:RHIRenderState;
import :Engine;
import :EngineDefinition;
import :RHIStruct;
import :Vector;
import :Color;
import :MathUtils;

export namespace Aether
{
/******************************************************************************
* RenderState
*******************************************************************************/
    struct DepthStencilStateDesc
    {
        bool                    bDepthEnable = true;
        bool                    bDepthWriteMask = true;
        ECompareFunction         eDepthFunc = ECompareFunction::Less;

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

    struct BlendStateDesc
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
            EBlendOperation eBlendOpColor =  EBlendOperation::Add;

            EBlendFactor     eSrcBlendAlpha = EBlendFactor::One;
            EBlendFactor     eDstBlendAlpha = EBlendFactor::Zero;
            EBlendOperation  eBlendOpAlpha  = EBlendOperation::Add;
            EColorWriteMask  bColorWriteMask = CWM_RGBA;
        };
        std::array<TargetBlendDesc, 8> stTargetBlend;
    };

    struct RenderStateDesc
    {
        RasterizerStateDesc     rasterizer = {};
        DepthStencilStateDesc   depthStencil = {};
        BlendStateDesc          blend = {};

        static const RenderStateDesc& Default3D();
        static const RenderStateDesc& Default2D();
        static const RenderStateDesc& PostProcess();
        static const RenderStateDesc& PostProcessAccumulate();
        static const RenderStateDesc& Skybox();
        static const RenderStateDesc& Particle();
        static const RenderStateDesc& WaterMark();

        static const RenderStateDesc& GBuffer();
        static const RenderStateDesc& ShadowCopyR();
        static const RenderStateDesc& ShadowCopyG();
        static const RenderStateDesc& ShadowCopyB();
        static const RenderStateDesc& ShadowCopyA();
        static const RenderStateDesc& Lighting();
        static const RenderStateDesc& DepthDisable();

        size_t   Hash() const;
        bool     operator==(RenderStateDesc const& rhs) const;
        bool     operator<(RenderStateDesc const& rhs) const;
    };

    class RHIRenderState
    {
    public:
        virtual ~RHIRenderState() {}

        RenderStateDesc const& GetRenderStateDesc() const { return m_stRenderStateDesc; }
        virtual bool IsTransparent() { return m_stRenderStateDesc.blend.stTargetBlend[0].bBlendEnable; }

    protected:
        RHIRenderState(AetherEngine* engine)
            : m_pEngine(engine)
        {
        }
        RHIRenderState(AetherEngine* engine, RenderStateDesc const& desc)
            : m_pEngine(engine), m_stRenderStateDesc(desc)
        {
        }
        RHIRenderState(AetherEngine* engine, RasterizerStateDesc const& rs_desc, DepthStencilStateDesc const& ds_desc, BlendStateDesc const& bs_desc)
            : m_pEngine(engine)
        {
            m_stRenderStateDesc.rasterizer = rs_desc;
            m_stRenderStateDesc.depthStencil = ds_desc;
            m_stRenderStateDesc.blend = bs_desc;
        }

        AetherEngine*           m_pEngine = nullptr;
        RenderStateDesc         m_stRenderStateDesc = {};
    };

    /******************************************************************************
    * RHISampler
    *******************************************************************************/
    struct SamplerDesc
    {
        ETexFilterOp     eFilterOp = ETexFilterOp::Min_Mag_Mip_Linear;
        ETexAddressMode  eAddrModeU = ETexAddressMode::Wrap;
        ETexAddressMode  eAddrModeV = ETexAddressMode::Wrap;
        ETexAddressMode  eAddrModeW = ETexAddressMode::Wrap;
        ECompareFunction eCompareFun = ECompareFunction::Never;
        Color           cBoarderColor = Color(0, 0, 0, 0);
        uint32_t        iMaxAnisotropy = 4;
        float           iMinLod = 0;
        float           iMaxLod = Math::FLOAT_MAX;
        float           iMipMapLodBias = 0.0f;

        static SamplerDesc GetSamplerDescByName(std::string const& name);
        static SamplerDesc PointSampler();
        static SamplerDesc LinearSampler();
        static SamplerDesc BilinearSampler();
        static SamplerDesc PrefilterMapSampler();
        static SamplerDesc AnisotropicSampler();
        static SamplerDesc SkyboxSampler();
        static SamplerDesc ShadowMapSampler();
        static SamplerDesc NoiseSampler();

        size_t   Hash() const;
        bool     operator==(SamplerDesc const& rhs) const;
        bool     operator<(SamplerDesc const& rhs) const;
    };

    class RHISampler
    {
    protected:
        RHISampler(AetherEngine* engine, SamplerDesc const& desc)
            : m_pEngine(engine), m_stSamplerDesc(desc)
        {
        }
        virtual ~RHISampler() = default;

    protected:
        AetherEngine* m_pEngine = nullptr;
        SamplerDesc m_stSamplerDesc = {};
    };
};
