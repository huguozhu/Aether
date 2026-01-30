export module Aether:Parameter;
import :Parameter;
import :Log;
import :Hash;

#include "Utils/Macros.h"

export namespace Aether
{
    std::unique_ptr<EffectVariable> CreateEffectVariable(EffectDataType data_type)
    {
        switch (data_type)
        {
        case EffectDataType::ConstantBuffer:
            return MakeUniquePtr<EffectVariableRHIGpuBuffer>();
        case EffectDataType::Buffer:
            return MakeUniquePtr<EffectVariableRHIShaderResourceView>();
        case EffectDataType::RWBuffer:
            return MakeUniquePtr<EffectVariableRHIUnorderedAccessView>();
        case EffectDataType::Texture:
        case EffectDataType::RWTexture:
            return MakeUniquePtr<EffectVariableRHITexture>();
        case EffectDataType::Sampler:
            return MakeUniquePtr<EffectVariableRHISampler>();
        case EffectDataType::SampledTexture:
            return nullptr; // it's just a placehold type, has no instance
        default:
            LOG_ERROR("invalid EffectDataType %d", data_type);
            return nullptr;
        }
    }

    std::unique_ptr<EffectVariable> EffectParam::ReadRenderVariable(EffectDataType data_type)
    {
        std::unique_ptr<EffectVariable> var = CreateEffectVariable(data_type);
        switch (data_type)
        {
        case EffectDataType::ConstantBuffer:
        {
            *var = RHIBufferPtr();
            break;
        }
        case EffectDataType::Buffer:
        {
            *var = RHIShaderResourceViewPtr();
            break;
        }
        case EffectDataType::RWBuffer:
        {
            *var = RHIUnorderedAccessViewPtr();
            break;
        }
        case EffectDataType::Texture:
        case EffectDataType::RWTexture:
        {
            *var = RHITexturePtr();
            break;
        }
        case EffectDataType::Sampler:
        {
            *var = RHISamplerPtr();
            break;
        }
        case EffectDataType::SampledTexture:
        {
            // do nothing
            break;
        }
        default:
            break;
        }
        return var;
    }

    /******************************************************************************
     * EffectParam
     ******************************************************************************/
    std::string GenerateSeedString(std::vector<EffectPredefine>& predefines)
    {
        std::string seedStr;
        if (predefines.size() > 0)
        {
            // TODO: optimize
            std::map<std::string, EffectPredefine> _pds;
            for (auto& pd : predefines)
            {
                _pds[pd.name] = pd;
            }

            size_t seed = 0;

            //LOG_INFO("GenerateSeedString:");
            for (auto& pd : _pds)
            {
                //LOG_INFO("  %s\t\t%s", pd.second.name.c_str(), pd.second.value.c_str());
                HashRange(seed, pd.second.value.begin(), pd.second.value.end());
            }

            char seedC[128];
            sprintf_s(seedC, sizeof(seedC), "_%zx", seed);
            seedStr = seedC;
        }
        return seedStr;
    }

    AResult EffectParam::UpdateConstantBuffer(const void* data, size_t size)
    {
        if (m_eDataType != EffectDataType::ConstantBuffer)
        {
            LOG_ERROR("UpdateConstantBuffer for param with type EffectDataType::ConstantBuffer, %s type is %u", m_szName.c_str(), m_eDataType);
            return ERR_INVALID_ARG;
        }

        RHIBufferPtr rb;
        GetValue(rb);
        if (!rb)
        {
            LOG_ERROR("ConstantBuffer %s is not created, it SHOULDN'T happen", m_szName.c_str());
            return ERR_UNKNOWN;
        }

        //AResult ret = rb->Update(data, (uint32_t)size);
        AResult ret = A_Success;
        if (AETHER_CHECKFAILED(ret))
            LOG_ERROR("update ConstantBuffer %s fail, ret=%x", m_szName.c_str(), ret);
        return ret;
    }
};
