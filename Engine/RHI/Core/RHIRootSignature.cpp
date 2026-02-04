module Aether:RHIRootSignature;
import :RHIRootSignature;
import :Engine;
import :EngineDefinition;
import :Hash;
import :Log;
import <string>;

export namespace Aether
{

	RHISamplerDesc RHISamplerDesc::GetSamplerDescByName(std::string const& name)
	{
        if (name == "point_sampler")                return PointSampler();
        else if (name == "linear_sampler")          return LinearSampler();
        //else if (name == "bilinear_sampler")        return BilinearSampler();
        //else if (name == "prefilter_map_sampler")   return PrefilterMapSampler();
        //else if (name == "anisotropic_sampler")     return AnisotropicSampler();
        //else if (name == "skybox_sampler")          return SkyboxSampler();
        //else if (name == "shadow_map_sampler")      return ShadowMapSampler();
        //else if (name == "noise_sampler")           return NoiseSampler();
        else
        {
            LOG_ERROR("Can not find %s SamplerDesc, return default Desc", name.c_str());
            return RHISamplerDesc();
        }
	}
    RHISamplerDesc RHISamplerDesc::PointSampler()
    {
        RHISamplerDesc desc;
        desc.filter = EFilterMode::MIN_MAG_MIP_POINT;
        desc.addressU = ETextureAddressMode::CLAMP;
        desc.addressV = ETextureAddressMode::CLAMP;
        desc.addressW = ETextureAddressMode::CLAMP;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "PointSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::LinearSampler()
    {
        RHISamplerDesc desc;
        desc.filter = EFilterMode::MIN_MAG_MIP_LINEAR;
        desc.addressU = ETextureAddressMode::WRAP;
        desc.addressV = ETextureAddressMode::WRAP;
        desc.addressW = ETextureAddressMode::WRAP;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "LinearSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::BilinearSampler()
    {
        RHISamplerDesc desc;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "BilinearSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::PrefilterMapSampler()
    {
        RHISamplerDesc desc;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "PrefilterMapSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::AnisotropicSampler()
    {
        RHISamplerDesc desc;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "AnisotropicSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::SkyboxSampler()
    {
        RHISamplerDesc desc;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "SkyboxSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::ShadowMapSampler()
    {
        RHISamplerDesc desc;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "ShadowMapSampler";
        return desc;
    }
    RHISamplerDesc RHISamplerDesc::NoiseSampler()
    {
        RHISamplerDesc desc;
        desc.shaderRegister = 0;
        desc.registerSpace = 0;
        desc.visibility = EShaderVisibility::All;
        desc.name = "NoiseSampler";
        return desc;
    }
};