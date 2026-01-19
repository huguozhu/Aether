module Aether:RHIStruct;
import :RHIStruct;
import :Log;
import :Hash;

namespace Aether
{

    //////////////////////////////////////////////////////////////////////////
    // SamplerDesc Declare
    //////////////////////////////////////////////////////////////////////////
    /*SamplerDesc SamplerDesc::GetSamplerDescByName(std::string const& name)
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
    */


    //////////////////////////////////////////////////////////////////////////
    // RenderStateDesc Declare
    //////////////////////////////////////////////////////////////////////////
    const RHIRenderStateDesc& RHIRenderStateDesc::Default3D()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = true;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::Default2D()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bBlendEnable = false;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::PostProcess()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::PostProcessAccumulate()
    {
        static RHIRenderStateDesc desc;
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
    const RHIRenderStateDesc& RHIRenderStateDesc::Skybox()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = true;
        desc.depthStencil.bDepthWriteMask = false;
        desc.depthStencil.eDepthFunc = ECompareFunction::Equal;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::Particle()
    {
        static RHIRenderStateDesc desc;
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
    const RHIRenderStateDesc& RHIRenderStateDesc::WaterMark()
    {
        static RHIRenderStateDesc desc;
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
    const RHIRenderStateDesc& RHIRenderStateDesc::GBuffer()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = true;
        desc.depthStencil.eDepthFunc = ECompareFunction::Equal;
        desc.depthStencil.bDepthWriteMask = false;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::ShadowCopyR()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Red;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::ShadowCopyG()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Green;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::ShadowCopyB()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Blue;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::ShadowCopyA()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        for (auto& rb : desc.blend.stTargetBlend) rb.bColorWriteMask = EColorWriteMask::CWM_Alpha;
        return desc;
    }
    const RHIRenderStateDesc& RHIRenderStateDesc::Lighting()
    {
        static RHIRenderStateDesc desc;
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
    const RHIRenderStateDesc& RHIRenderStateDesc::DepthDisable()
    {
        static RHIRenderStateDesc desc;
        desc.depthStencil.bDepthEnable = false;
        desc.depthStencil.eDepthFunc = ECompareFunction::Always;
        desc.depthStencil.bDepthWriteMask = false;
        return desc;
    }
    //////////////////////////////////////////////////////////////////////////

    size_t RHIRenderStateDesc::Hash() const
    {
        char* begin = (char*)(this); 
        char* end = begin + sizeof(*this); 
        return HashRange(begin, end); 
    }
    bool RHIRenderStateDesc::operator==(RHIRenderStateDesc const& rhs) const
    {
        return this->Hash() == rhs.Hash();
    }
    bool RHIRenderStateDesc::operator<(RHIRenderStateDesc const& rhs) const
    {
        return this->Hash() < rhs.Hash();
    }
};
