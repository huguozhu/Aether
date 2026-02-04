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

        /*const char* D3D12Translate::TranslateVertexElementUsageSemantic(VertexElementUsage usage)
        {
            static const char* POSITION = "POSITION";
            static const char* TEXCOORD = "TEXCOORD";
            static const char* NORMAL = "NORMAL";
            static const char* COLOR = "COLOR";
            static const char* BLENDWEIGHT = "BLENDWEIGHT";
            static const char* BLENDINDEX = "BLENDINDEX";
            static const char* TANGENT = "TANGENT";
            static const char* BINORMAL = "BINORMAL";
            static const char* INSTANCE = "INSTANCE";
            static const char* UNKNOWN = "UNKNOWN";
            const char* res = POSITION;
            switch (usage)
            {
            case VertexElementUsage::Position:      res = POSITION;     break;
            case VertexElementUsage::TexCoord:      res = TEXCOORD;     break;
            case VertexElementUsage::Normal:        res = NORMAL;       break;
            case VertexElementUsage::Color:         res = COLOR;        break;
            case VertexElementUsage::BlendWeight:   res = BLENDWEIGHT;  break;
            case VertexElementUsage::BlendIndex:    res = BLENDINDEX;   break;
            case VertexElementUsage::Tangent:       res = TANGENT;      break;
            case VertexElementUsage::Binormal:      res = BINORMAL;     break;
            case VertexElementUsage::Instance:      res = INSTANCE;     break;

            default:
                res = UNKNOWN;
            }
            return res;
        }*/

        D3D12_PRIMITIVE_TOPOLOGY D3D12Translate::TranslatePrimitiveTopology(EMeshTopologyType type)
        {
            D3D12_PRIMITIVE_TOPOLOGY res = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
            switch (type)
            {
            case EMeshTopologyType::Points:        res = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;       break;
            case EMeshTopologyType::Lines:         res = D3D_PRIMITIVE_TOPOLOGY_LINELIST;        break;
            case EMeshTopologyType::Line_Strip:    res = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP;       break;
            case EMeshTopologyType::Triangles:     res = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;    break;
            case EMeshTopologyType::Triangle_Strip:res = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;   break;
            }
            return res;
        }
        D3D12_PRIMITIVE_TOPOLOGY_TYPE D3D12Translate::TranslatePrimitiveTopologyType(EMeshTopologyType type)
        {
            D3D12_PRIMITIVE_TOPOLOGY_TYPE res = D3D12_PRIMITIVE_TOPOLOGY_TYPE_UNDEFINED;
            switch (type)
            {
            case EMeshTopologyType::Points:        res = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;       break;
            case EMeshTopologyType::Lines:         res = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;        break;
            case EMeshTopologyType::Line_Strip:    res = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;       break;
            case EMeshTopologyType::Triangles:     res = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;    break;
            case EMeshTopologyType::Triangle_Strip:res = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;   break;
            }
            return res;
        }

        D3D12_CULL_MODE D3D12Translate::TranslateCullMode(ECullMode CullMode)
        {
            D3D12_CULL_MODE res = D3D12_CULL_MODE_NONE;
            switch (CullMode)
            {
            case ECullMode::Back:   res = D3D12_CULL_MODE_BACK;  break;
            case ECullMode::Front:  res = D3D12_CULL_MODE_FRONT; break;
            default:        res = D3D12_CULL_MODE_NONE;  break;
            };
            return res;
        }
        D3D12_FILL_MODE D3D12Translate::TranslateFillMode(EFillMode FillMode)
        {
            D3D12_FILL_MODE res = D3D12_FILL_MODE_SOLID;
            switch (FillMode)
            {
            case EFillMode::Wireframe:   res = D3D12_FILL_MODE_WIREFRAME; break;
            case EFillMode::Solid:       res = D3D12_FILL_MODE_SOLID;     break;
            };
            return res;
        }
        D3D12_STENCIL_OP D3D12Translate::TranslateStencilOp(EStencilOperation StencilOp)
        {
            D3D12_STENCIL_OP res = D3D12_STENCIL_OP_KEEP;
            switch (StencilOp)
            {
            case EStencilOperation::Zero:               res = D3D12_STENCIL_OP_ZERO;    break;
            case EStencilOperation::Replace:            res = D3D12_STENCIL_OP_REPLACE; break;
            case EStencilOperation::SaturatedIncrement: res = D3D12_STENCIL_OP_INCR_SAT; break;
            case EStencilOperation::SaturatedDecrement: res = D3D12_STENCIL_OP_DECR_SAT; break;
            case EStencilOperation::Invert:             res = D3D12_STENCIL_OP_INVERT;  break;
            case EStencilOperation::Increment:          res = D3D12_STENCIL_OP_INCR;    break;
            case EStencilOperation::Decrement:          res = D3D12_STENCIL_OP_DECR;    break;
            default:                                    res = D3D12_STENCIL_OP_KEEP;    break;
            };
            return res;
        }
        D3D12_BLEND_OP D3D12Translate::TranslateBlendOp(EBlendOperation BlendOp)
        {
            D3D12_BLEND_OP res = D3D12_BLEND_OP_ADD;
            switch (BlendOp)
            {
            case EBlendOperation::Subtract:          res = D3D12_BLEND_OP_SUBTRACT;     break;
            case EBlendOperation::Min:               res = D3D12_BLEND_OP_MIN;          break;
            case EBlendOperation::Max:               res = D3D12_BLEND_OP_MAX;          break;
            case EBlendOperation::ReverseSubtract:   res = D3D12_BLEND_OP_REV_SUBTRACT; break;
            default:                                res = D3D12_BLEND_OP_ADD;           break;
            };
            return res;
        }
        D3D12_BLEND D3D12Translate::TranslateBlendFactor(EBlendFactor BlendFactor)
        {
            D3D12_BLEND res = D3D12_BLEND_ZERO;
            switch (BlendFactor)
            {
            case EBlendFactor::Zero:           res = D3D12_BLEND_ZERO;              break;
            case EBlendFactor::One:            res = D3D12_BLEND_ONE;               break;
            case EBlendFactor::SrcColor:       res = D3D12_BLEND_SRC_COLOR;         break;
            case EBlendFactor::InvSrcColor:    res = D3D12_BLEND_INV_SRC_COLOR;     break;
            case EBlendFactor::SrcAlpha:       res = D3D12_BLEND_SRC_ALPHA;         break;
            case EBlendFactor::InvSrcAlpha:    res = D3D12_BLEND_INV_SRC_ALPHA;     break;
            case EBlendFactor::DstAlpha:       res = D3D12_BLEND_DEST_ALPHA;        break;
            case EBlendFactor::InvDstAlpha:    res = D3D12_BLEND_INV_DEST_ALPHA;    break;
            case EBlendFactor::DstColor:       res = D3D12_BLEND_DEST_COLOR;        break;
            case EBlendFactor::InvDstColor:    res = D3D12_BLEND_INV_DEST_COLOR;    break;

            case EBlendFactor::SrcAlphaSat:    res = D3D12_BLEND_SRC_ALPHA_SAT;     break;
            case EBlendFactor::BlendFactor:    res = D3D12_BLEND_BLEND_FACTOR;      break;
            case EBlendFactor::InvBlendFactor: res = D3D12_BLEND_INV_BLEND_FACTOR;  break;

            case EBlendFactor::Src1Color:      res = D3D12_BLEND_SRC1_COLOR;        break;
            case EBlendFactor::InvSrc1Color:   res = D3D12_BLEND_INV_SRC1_COLOR;    break;
            case EBlendFactor::Src1Alpha:      res = D3D12_BLEND_SRC1_ALPHA;        break;
            case EBlendFactor::InvSrc1Alpha:   res = D3D12_BLEND_INV_SRC1_ALPHA;    break;

            default:                            res = D3D12_BLEND_ZERO;             break;
            };
            return res;
        }

        D3D12_HIT_GROUP_TYPE TranslateHitGroupType(EHitGroupType type)
        {
            D3D12_HIT_GROUP_TYPE res = D3D12_HIT_GROUP_TYPE_TRIANGLES;
            switch (type)
            {
            case EHitGroupType::Triangles:              
                res = D3D12_HIT_GROUP_TYPE_TRIANGLES;               break;
            case EHitGroupType::Procedural_Primitive:  
                res = D3D12_HIT_GROUP_TYPE_PROCEDURAL_PRIMITIVE;    break;
            default:                                    
                res = D3D12_HIT_GROUP_TYPE_TRIANGLES;             break;
            };
            return res;
        }











        PixelFormat TranslateFromPlatformFormat(DXGI_FORMAT format)
        {
            PixelFormat res = PixelFormat::Unknown;
            switch (format)
            {
                // 8-bit
            case DXGI_FORMAT_R8_UNORM:              res = PixelFormat::R8_UNORM;            break;
            case DXGI_FORMAT_R8_UINT:               res = PixelFormat::R8_UINT;             break;

                // 16-bit
            case DXGI_FORMAT_R16_UINT:              res = PixelFormat::R16_UINT;            break;
            case DXGI_FORMAT_R16_SINT:              res = PixelFormat::R16_SINT;            break;
            case DXGI_FORMAT_R8G8_UINT:             res = PixelFormat::R8G8_UINT;           break;
            case DXGI_FORMAT_R8G8_SINT:             res = PixelFormat::R8G8_SINT;           break;

                // 32-bit
            case DXGI_FORMAT_R32_FLOAT:             res = PixelFormat::R32F;                break;
            case DXGI_FORMAT_R32_UINT:              res = PixelFormat::R32_UINT;            break;
            case DXGI_FORMAT_R32_SINT:              res = PixelFormat::R32_SINT;            break;
            case DXGI_FORMAT_R8G8B8A8_UNORM:        res = PixelFormat::R8G8B8A8_UNORM;      break;
            case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:   res = PixelFormat::R8G8B8A8_UNORM_SRGB; break;
            case DXGI_FORMAT_B8G8R8A8_UNORM:        res = PixelFormat::B8G8R8A8_UNORM;      break;
            case DXGI_FORMAT_R8G8B8A8_UINT:         res = PixelFormat::R8G8B8A8_UINT;       break;
            case DXGI_FORMAT_R16G16_SNORM:          res = PixelFormat::R16G16_SNORM;        break;
                // 64-bit
            case DXGI_FORMAT_R32G32_FLOAT:          res = PixelFormat::R32G32F;             break;
            case DXGI_FORMAT_R16G16B16A16_UNORM:    res = PixelFormat::R16G16B16A16_UNORM;  break;
            case DXGI_FORMAT_R16G16B16A16_UINT:     res = PixelFormat::R16G16B16A16_UINT;   break;
            case DXGI_FORMAT_R16G16B16A16_SINT:     res = PixelFormat::R16G16B16A16_SINT;   break;
            case DXGI_FORMAT_R16G16B16A16_FLOAT:    res = PixelFormat::R16G16B16A16_FLOAT;  break;

                // 128-bit
            case DXGI_FORMAT_R32G32B32A32_FLOAT:    res = PixelFormat::R32G32B32A32_FLOAT;  break;

                // depth
            case DXGI_FORMAT_R16_TYPELESS:          res = PixelFormat::D16;                 break;
            case DXGI_FORMAT_R24G8_TYPELESS:        res = PixelFormat::D24S8;               break;
            case DXGI_FORMAT_R32_TYPELESS:          res = PixelFormat::D32F;                break;
            }
            return res;
        }
        DXGI_FORMAT TranslateToPlatformFormat(PixelFormat format)
        {
            DXGI_FORMAT res = DXGI_FORMAT_UNKNOWN;
            switch (format)
            {
                // 8-bit
            case PixelFormat::R8_UNORM:             res = DXGI_FORMAT_R8_UNORM;             break;
            case PixelFormat::R8_UINT:              res = DXGI_FORMAT_R8_UINT;              break;

                // 16-bit
            case PixelFormat::R16_UINT:             res = DXGI_FORMAT_R16_UINT;             break;
            case PixelFormat::R16_SINT:             res = DXGI_FORMAT_R16_SINT;             break;
            case PixelFormat::R8G8_UINT:            res = DXGI_FORMAT_R8G8_UINT;            break;
            case PixelFormat::R8G8_SINT:            res = DXGI_FORMAT_R8G8_SINT;            break;
            case PixelFormat::R8G8_UNORM:           res = DXGI_FORMAT_R8G8_UNORM;           break;

                // 32-bit
            case PixelFormat::R32F:                 res = DXGI_FORMAT_R32_FLOAT;            break;
            case PixelFormat::R32_UINT:             res = DXGI_FORMAT_R32_UINT;             break;
            case PixelFormat::R32_SINT:             res = DXGI_FORMAT_R32_SINT;             break;
            case PixelFormat::R8G8B8A8_UNORM:       res = DXGI_FORMAT_R8G8B8A8_UNORM;       break;
            case PixelFormat::R8G8B8A8_UNORM_SRGB:  res = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;  break;
            case PixelFormat::B8G8R8A8_UNORM:       res = DXGI_FORMAT_B8G8R8A8_UNORM;       break;
            case PixelFormat::R8G8B8A8_UINT:        res = DXGI_FORMAT_R8G8B8A8_UINT;        break;
            case PixelFormat::R16G16_SNORM:         res = DXGI_FORMAT_R16G16_SNORM;         break;
            case PixelFormat::R8G8_B8G8_UNORM:      res = DXGI_FORMAT_R8G8_B8G8_UNORM;      break;
            case PixelFormat::G8R8_G8B8_UNORM:      res = DXGI_FORMAT_G8R8_G8B8_UNORM;      break;

                // 64-bit
            case PixelFormat::R32G32F:              res = DXGI_FORMAT_R32G32_FLOAT;         break;
            case PixelFormat::R16G16B16A16_UNORM:   res = DXGI_FORMAT_R16G16B16A16_UNORM;   break;
            case PixelFormat::R16G16B16A16_UINT:    res = DXGI_FORMAT_R16G16B16A16_UINT;    break;
            case PixelFormat::R16G16B16A16_SINT:    res = DXGI_FORMAT_R16G16B16A16_SINT;    break;
            case PixelFormat::R16G16B16A16_FLOAT:   res = DXGI_FORMAT_R16G16B16A16_FLOAT;   break;

                // 128-bit
            case PixelFormat::R32G32B32A32_FLOAT:   res = DXGI_FORMAT_R32G32B32A32_FLOAT;   break;

                // depth
            case PixelFormat::D16:                  res = DXGI_FORMAT_R16_TYPELESS;         break;
            case PixelFormat::D24S8:                res = DXGI_FORMAT_R24G8_TYPELESS;       break;
            case PixelFormat::D32F:                 res = DXGI_FORMAT_R32_TYPELESS;         break;
            }
            return res;
        }
        DXGI_FORMAT TranslateToPlatformFormat(VertexFormat format)
        {
            switch (format)
            {
            case VertexFormat::Unknown:            return DXGI_FORMAT_UNKNOWN;
            case VertexFormat::UChar2:             return DXGI_FORMAT_R8G8_UINT;
                //case VertexFormat::UChar3:           return DXGI_FORMAT_R8G8B8A8_UINT;
            case VertexFormat::UChar4:             return DXGI_FORMAT_R8G8B8A8_UINT;
            case VertexFormat::Char2:              return DXGI_FORMAT_R8G8_SINT;
                //case VertexFormat::Char3:            return DXGI_FORMAT_R8G8B8A8_SINT;
            case VertexFormat::Char4:              return DXGI_FORMAT_R8G8B8A8_SINT;
            case VertexFormat::UChar2Normalized:   return DXGI_FORMAT_R8G8_UNORM;
                //case VertexFormat::UChar3Normalized: return DXGI_FORMAT_R8G8B8A8_SNORM;
            case VertexFormat::UChar4Normalized:   return DXGI_FORMAT_R8G8B8A8_SNORM;
            case VertexFormat::Char2Normalized:    return DXGI_FORMAT_R8G8_SNORM;
                //case VertexFormat::Char3Normalized:  return DXGI_FORMAT_R8G8B8A8_SNORM;
            case VertexFormat::Char4Normalized:    return DXGI_FORMAT_R8G8B8A8_SNORM;

            case VertexFormat::UShort2:            return DXGI_FORMAT_R16G16_UINT;
                //case VertexFormat::UShort3:          return DXGI_FORMAT_R16G16B16A16_UINT;
            case VertexFormat::UShort4:            return DXGI_FORMAT_R16G16B16A16_UINT;
            case VertexFormat::Short2:             return DXGI_FORMAT_R16G16_SINT;
                //case VertexFormat::Short3:           return DXGI_FORMAT_R16G16B16A16_SINT;
            case VertexFormat::Short4:             return DXGI_FORMAT_R16G16B16A16_SINT;
            case VertexFormat::UShort2Normalized:  return DXGI_FORMAT_R16G16_UNORM;
                //case VertexFormat::UShort3Normalized:return DXGI_FORMAT_R16G16B16A16_UNORM;
            case VertexFormat::UShort4Normalized:  return DXGI_FORMAT_R16G16B16A16_UNORM;
            case VertexFormat::Short2Normalized:   return DXGI_FORMAT_R16G16_SNORM;
                //case VertexFormat::Short3Normalized: return DXGI_FORMAT_R16G16B16A16_SNORM;
            case VertexFormat::Short4Normalized:   return DXGI_FORMAT_R16G16B16A16_SNORM;

            case VertexFormat::Half2:              return DXGI_FORMAT_R16G16_FLOAT;
                //case VertexFormat::Half3:              return DXGI_FORMAT_R16G16B16_FLOAT;
            case VertexFormat::Half4:              return DXGI_FORMAT_R16G16B16A16_FLOAT;
            case VertexFormat::Float:              return DXGI_FORMAT_R32_FLOAT;
            case VertexFormat::Float2:             return DXGI_FORMAT_R32G32_FLOAT;
            case VertexFormat::Float3:             return DXGI_FORMAT_R32G32B32_FLOAT;
            case VertexFormat::Float4:             return DXGI_FORMAT_R32G32B32A32_FLOAT;
            case VertexFormat::UInt:               return DXGI_FORMAT_R32_UINT;
            case VertexFormat::UInt2:              return DXGI_FORMAT_R32G32_UINT;
            case VertexFormat::UInt3:              return DXGI_FORMAT_R32G32B32_UINT;
            case VertexFormat::UInt4:              return DXGI_FORMAT_R32G32B32A32_UINT;
            case VertexFormat::Int:                return DXGI_FORMAT_R32_SINT;
            case VertexFormat::Int2:               return DXGI_FORMAT_R32G32_SINT;
            case VertexFormat::Int3:               return DXGI_FORMAT_R32G32B32_SINT;
            case VertexFormat::Int4:               return DXGI_FORMAT_R32G32B32A32_SINT;
            default:  return DXGI_FORMAT_UNKNOWN;
            }
        }

        D3D_ROOT_SIGNATURE_VERSION TranslateRootSignatureVersion(ERootSignatureVersion v)
        {
            switch (v)
            {
            case ERootSignatureVersion::Version_1_0: return D3D_ROOT_SIGNATURE_VERSION_1_0;
            case ERootSignatureVersion::Version_1_1: return D3D_ROOT_SIGNATURE_VERSION_1_1;
            case ERootSignatureVersion::Version_1_2: return D3D_ROOT_SIGNATURE_VERSION_1_2;
            }
            return D3D_ROOT_SIGNATURE_VERSION_1_0;
        }

	};
};