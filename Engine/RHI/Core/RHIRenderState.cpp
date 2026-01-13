module Aether:RHIRenderState;
import :RHIRenderState;
import :Log;
import :Hash;

namespace Aether
{

    //////////////////////////////////////////////////////////////////////////
    // SamplerDesc Declare
    //////////////////////////////////////////////////////////////////////////
    SamplerDesc SamplerDesc::GetSamplerDescByName(std::string const& name)
    {
        if (name == "point_sampler")           return PointSampler();
        else if (name == "linear_sampler")          return LinearSampler();
        else if (name == "bilinear_sampler")        return BilinearSampler();
        else if (name == "prefilter_map_sampler")   return PrefilterMapSampler();
        else if (name == "anisotropic_sampler")     return AnisotropicSampler();
        else if (name == "skybox_sampler")          return SkyboxSampler();
        else if (name == "shadow_map_sampler")      return ShadowMapSampler();
        else if (name == "noise_sampler")           return NoiseSampler();
        else
        {
            LOG_ERROR("Can not find %s SamplerDesc, return default Desc", name.c_str());
            return SamplerDesc();
        }
    }
    SamplerDesc SamplerDesc::PointSampler()
    {
        static SamplerDesc desc;
        desc.eFilterOp = ETexFilterOp::Min_Mag_Mip_Point;
        desc.eAddrModeU = ETexAddressMode::Clamp;
        desc.eAddrModeV = ETexAddressMode::Clamp;
        desc.eCompareFun = ECompareFunction::Always;
        return desc;
    }
    SamplerDesc SamplerDesc::LinearSampler()
    {
        static SamplerDesc desc;
        desc.eCompareFun = ECompareFunction::Always;
        return desc;
    }
    SamplerDesc SamplerDesc::BilinearSampler()
    {
        static SamplerDesc desc;
        desc.eFilterOp = ETexFilterOp::Min_Mag_Linear_Mip_Point;
        return desc;
    }
    SamplerDesc SamplerDesc::PrefilterMapSampler()
    {
        static SamplerDesc desc;
        desc.eAddrModeU = ETexAddressMode::Clamp;
        desc.eAddrModeV = ETexAddressMode::Clamp;
        desc.eAddrModeW = ETexAddressMode::Clamp;
        desc.eCompareFun = ECompareFunction::Always;
        return desc;
    }
    SamplerDesc SamplerDesc::AnisotropicSampler()
    {
        static SamplerDesc desc;
        desc.eFilterOp = ETexFilterOp::Anisotropic;
        desc.eAddrModeU = ETexAddressMode::Clamp;
        desc.eAddrModeV = ETexAddressMode::Clamp;
        return desc;
    }
    SamplerDesc SamplerDesc::SkyboxSampler()
    {
        static SamplerDesc desc;
        desc.eAddrModeU = ETexAddressMode::Clamp;
        desc.eAddrModeV = ETexAddressMode::Clamp;
        desc.eAddrModeW = ETexAddressMode::Clamp;
        return desc;
    }
    SamplerDesc SamplerDesc::ShadowMapSampler()
    {
        static SamplerDesc desc;
        desc.eAddrModeU = ETexAddressMode::Border;
        desc.eAddrModeV = ETexAddressMode::Border;
        desc.eAddrModeW = ETexAddressMode::Border;
        return desc;
    }
    SamplerDesc SamplerDesc::NoiseSampler()
    {
        static SamplerDesc desc;
        desc.eFilterOp = ETexFilterOp::Min_Mag_Mip_Point;
        desc.eAddrModeU = ETexAddressMode::Mirror;
        desc.eAddrModeV = ETexAddressMode::Mirror;
        return desc;
    }
    //////////////////////////////////////////////////////////////////////////
    // RenderStateDesc Declare
    //////////////////////////////////////////////////////////////////////////
    const RenderStateDesc& RenderStateDesc::Default3D()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = true;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::Default2D()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bBlendEnable = false;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::PostProcess()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::PostProcessAccumulate()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;

        for (uint32_t i = 0; i < 8; ++i)
        {
            desc.blend.stTargetBlend[i].bBlendEnable = true;
            desc.blend.stTargetBlend[i].eBlendOpColor = EBlendOperation::Add;
            desc.blend.stTargetBlend[i].eSrcBlendColor = EBlendFactor::One;
            desc.blend.stTargetBlend[i].eDstBlendColor = EBlendFactor::One;
        }
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::Skybox()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = true;
        desc.depthStencil.bDepthWriteMask = false;
        desc.depthStencil.eDepthFunc = ECompareFunction::Equal;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::Particle()
    {
        static RenderStateDesc desc;
        desc.rasterizer.eCullMode = ECullMode::None;
        desc.depthStencil.bDepthEnable = true;
        desc.depthStencil.bDepthWriteMask = false;

        desc.blend.bIndependentBlendEnable = false;
        for (auto& rb : desc.blend.stTargetBlend)
        {
            rb.bBlendEnable = true;
            rb.eSrcBlendColor = EBlendFactor::SrcAlpha;
            rb.eDstBlendColor = EBlendFactor::InvSrcAlpha;

            rb.eSrcBlendAlpha = EBlendFactor::DstAlpha;
            rb.eDstBlendAlpha = EBlendFactor::One;
        }
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::WaterMark()
    {
        static RenderStateDesc desc;
        desc.rasterizer.eCullMode = ECullMode::None;
        desc.depthStencil.bDepthEnable = false;
        desc.depthStencil.bDepthWriteMask = false;

        desc.blend.bIndependentBlendEnable = false;
        for (auto& rb : desc.blend.stTargetBlend)
        {
            rb.bBlendEnable = true;
            rb.eSrcBlendColor = EBlendFactor::SrcAlpha;
            rb.eDstBlendColor = EBlendFactor::InvSrcAlpha;

            rb.eSrcBlendAlpha = EBlendFactor::DstAlpha;
            rb.eDstBlendAlpha = EBlendFactor::One;
        }
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::GBuffer()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = true;
        desc.depthStencil.eDepthFunc = ECompareFunction::Equal;
        desc.depthStencil.bDepthWriteMask = false;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::ShadowCopyR()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Red;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::ShadowCopyG()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Green;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::ShadowCopyB()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Blue;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::ShadowCopyA()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Alpha;
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::Lighting()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        desc.depthStencil.bDepthWriteMask = false;

        desc.rasterizer.bDepthClip = false;
        desc.rasterizer.eCullMode = ECullMode::Front;

        for (auto& rb : desc.blend.stTargetBlend)
        {
            rb.bBlendEnable = false;
            rb.eBlendOpColor = EBlendOperation::Add;
            rb.eSrcBlendColor = EBlendFactor::One;
            rb.eDstBlendColor = EBlendFactor::One;
            rb.eBlendOpAlpha = EBlendOperation::Add;
            rb.eSrcBlendAlpha = EBlendFactor::One;
            rb.eDstBlendAlpha = EBlendFactor::Zero;
        }
        return desc;
    }
    const RenderStateDesc& RenderStateDesc::DepthDisable()
    {
        static RenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        desc.depthStencil.eDepthFunc = ECompareFunction::Always;
        desc.depthStencil.bDepthWriteMask = false;
        return desc;
    }
    //////////////////////////////////////////////////////////////////////////

    size_t RenderStateDesc::Hash() const
    {
        char* begin = (char*)(this); 
        char* end = begin + sizeof(*this); 
        return HashRange(begin, end); 
    }
    bool RenderStateDesc::operator==(RenderStateDesc const& rhs) const
    {
        return this->Hash() == rhs.Hash();
    }
    bool RenderStateDesc::operator<(RenderStateDesc const& rhs) const
    {
        return this->Hash() < rhs.Hash();
    }

    size_t SamplerDesc::Hash() const
    {
        char* begin = (char*)(this);
        char* end = begin + sizeof(*this);
        return HashRange(begin, end);
    }
    bool SamplerDesc::operator==(SamplerDesc const& rhs) const
    {
        return this->Hash() == rhs.Hash();
    }
    bool SamplerDesc::operator<(SamplerDesc const& rhs) const
    {
        return this->Hash() < rhs.Hash();
    }
};
