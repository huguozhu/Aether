module Aether:D3D12Translate;
import :D3D12Translate;
import :RHIStruct;
import :Log;
import :D3D12Definition;
import std;

namespace Aether
{
    namespace D3D12Translate
	{
		D3D12_COMMAND_LIST_TYPE TranslateCommandListType(ECommandListType type)
		{
            switch (type)
            {
            case ECommandListType::Graphics:
                return D3D12_COMMAND_LIST_TYPE_DIRECT;
            case ECommandListType::Compute:
                return D3D12_COMMAND_LIST_TYPE_COMPUTE;
            case ECommandListType::Copy:
                return D3D12_COMMAND_LIST_TYPE_COPY;
            case ECommandListType::Bundle:
                return D3D12_COMMAND_LIST_TYPE_BUNDLE;
            default:
            {
                LOG_ERROR("Invalid ECommandListType type=%d", (uint32_t)type);
                throw std::runtime_error("Unknown command list type");
            }
            }
		}

        D3D12_ROOT_PARAMETER_TYPE TranslateParameterType(ERootParameterType type)
        {
            switch (type)
            {
            case ERootParameterType::DescriptorTable:   return D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            case ERootParameterType::Constant32Bit:     return D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            case ERootParameterType::ConstantBuffer:    return D3D12_ROOT_PARAMETER_TYPE_CBV;
            case ERootParameterType::ShaderResourceView:return D3D12_ROOT_PARAMETER_TYPE_SRV;
            case ERootParameterType::UnorderedAccessView:   return D3D12_ROOT_PARAMETER_TYPE_UAV;
            //case ERootParameterType::Sampler:           return D3D12_COMMAND_LIST_TYPE_BUNDLE;

            default:
            {
                LOG_ERROR("Invalid ERootParameterType type=%d", (uint32_t)type);
                throw std::runtime_error("Unknown Root Parameter type");
            }
            }
        }
        D3D12_SHADER_VISIBILITY TranslateShaderVisibility(EShaderVisibility visibility)
        {
            switch (visibility)
            {
            case EShaderVisibility::All:    return D3D12_SHADER_VISIBILITY_ALL;
            case EShaderVisibility::Vertex: return D3D12_SHADER_VISIBILITY_VERTEX;
            case EShaderVisibility::Pixel:  return D3D12_SHADER_VISIBILITY_PIXEL;
            case EShaderVisibility::Domain: return D3D12_SHADER_VISIBILITY_DOMAIN;
            case EShaderVisibility::Hull:   return D3D12_SHADER_VISIBILITY_HULL;
            case EShaderVisibility::Geometry:  return D3D12_SHADER_VISIBILITY_GEOMETRY;
            //case EShaderVisibility::Computer:  return D3D12_COMMAND_LIST_TYPE_BUNDLE;
            case EShaderVisibility::Amplification: return D3D12_SHADER_VISIBILITY_AMPLIFICATION;
            case EShaderVisibility::Mesh:   return D3D12_SHADER_VISIBILITY_MESH;

            default:
            {
                LOG_ERROR("Invalid EShaderVisibility type=%d", (uint32_t)visibility);
                throw std::runtime_error("Unknown EShaderVisibility type");
            }
            }
        }
        D3D12_DESCRIPTOR_RANGE_TYPE TranslateDescriptorRangeType(EDescriptorRangeType type)
        {
            switch (type)
            {
            case EDescriptorRangeType::Srv:     return D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            case EDescriptorRangeType::Uav:     return D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            case EDescriptorRangeType::Cbv:     return D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
            case EDescriptorRangeType::Sampler: return D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;

            default:
            {
                LOG_ERROR("Invalid EDescriptorRangeType type=%d", (uint32_t)type);
                throw std::runtime_error("Unknown EDescriptorRangeType type");
            }
            }
        }
        D3D12_FILTER TranslateFilter(EFilterMode filter)
        {
            switch (filter)
            {
            case EFilterMode::MIN_MAG_MIP_POINT:                return D3D12_FILTER_MIN_MAG_MIP_POINT;
            case EFilterMode::MIN_MAG_POINT_MIP_LINEAR:         return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
            case EFilterMode::MIN_POINT_MAG_LINEAR_MIP_POINT:   return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
            case EFilterMode::MIN_POINT_MAG_MIP_LINEAR:         return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
            case EFilterMode::MIN_LINEAR_MAG_MIP_POINT:         return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;

            case EFilterMode::MIN_LINEAR_MAG_POINT_MIP_LINEAR:  return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
            case EFilterMode::MIN_MAG_LINEAR_MIP_POINT:         return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            case EFilterMode::MIN_MAG_MIP_LINEAR:               return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
            case EFilterMode::ANISOTROPIC:                      return D3D12_FILTER_ANISOTROPIC;
            case EFilterMode::COMPARISON_MIN_MAG_MIP_POINT:     return D3D12_FILTER_COMPARISON_MIN_MAG_MIP_POINT;

            case EFilterMode::COMPARISON_MIN_MAG_MIP_LINEAR:    return D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
            case EFilterMode::COMPARISON_ANISOTROPIC:           return D3D12_FILTER_COMPARISON_ANISOTROPIC;
            case EFilterMode::MINIMUM_MIN_MAG_MIP_POINT:        return D3D12_FILTER_MINIMUM_MIN_MAG_MIP_POINT;
            case EFilterMode::MINIMUM_MIN_MAG_MIP_LINEAR:       return D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
            case EFilterMode::MINIMUM_ANISOTROPIC:              return D3D12_FILTER_MINIMUM_ANISOTROPIC;

            case EFilterMode::MAXIMUM_MIN_MAG_MIP_POINT:        return D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_POINT;
            case EFilterMode::MAXIMUM_MIN_MAG_MIP_LINEAR:       return D3D12_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR;
            case EFilterMode::MAXIMUM_ANISOTROPIC:              return D3D12_FILTER_MAXIMUM_ANISOTROPIC;

            default:
            {
                LOG_ERROR("Invalid EFilterMode filter=%d", (uint32_t)filter);
                throw std::runtime_error("Unknown EFilterMode filter");
            }
            }
        }
        D3D12_TEXTURE_ADDRESS_MODE TranslateAddressMode(ETextureAddressMode mode)
        {
            switch (mode)
            {
            case ETextureAddressMode::WRAP:         return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
            case ETextureAddressMode::MIRROR:       return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
            case ETextureAddressMode::CLAMP:        return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            case ETextureAddressMode::BORDER:       return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
            case ETextureAddressMode::MIRROR_ONCE:  return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
            default:
            {
                LOG_ERROR("Invalid ETextureAddressMode type=%d", (uint32_t)mode);
                throw std::runtime_error("Unknown ETextureAddressMode type");
            }
            }
        }

        D3D12_COMPARISON_FUNC TranslateCompareFunction(ECompareFunction func)
        {
            switch (func)
            {
            case ECompareFunction::Less:         return D3D12_COMPARISON_FUNC_LESS;
            case ECompareFunction::LessEqual:    return D3D12_COMPARISON_FUNC_LESS_EQUAL;
            case ECompareFunction::Greater:      return D3D12_COMPARISON_FUNC_GREATER;
            case ECompareFunction::GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
            case ECompareFunction::Equal:        return D3D12_COMPARISON_FUNC_EQUAL;
            case ECompareFunction::NotEqual:     return D3D12_COMPARISON_FUNC_NOT_EQUAL;
            case ECompareFunction::Never:        return D3D12_COMPARISON_FUNC_NEVER;
            case ECompareFunction::Always:       return D3D12_COMPARISON_FUNC_ALWAYS;
            case ECompareFunction::None:         return D3D12_COMPARISON_FUNC_NONE;
            default:
            {
                LOG_ERROR("Invalid ECompareFunction func=%d", (uint32_t)func);
                throw std::runtime_error("Unknown ECompareFunction func");
            }
            }
        }

	};
};