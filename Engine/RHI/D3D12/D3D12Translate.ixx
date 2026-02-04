export module Aether:D3D12Translate;
import :RHIStruct;
import :D3D12Definition;
import :RHIRootSignature;
import :Format;

namespace Aether
{
	namespace D3D12Translate
	{
		D3D12_COMMAND_LIST_TYPE			TranslateCommandListType(ECommandListType type);

		D3D12_ROOT_PARAMETER_TYPE		TranslateParameterType(ERootParameterType type);
		D3D12_SHADER_VISIBILITY			TranslateShaderVisibility(EShaderVisibility visibility);
		D3D12_DESCRIPTOR_RANGE_TYPE		TranslateDescriptorRangeType(EDescriptorRangeType type);
		D3D12_FILTER					TranslateFilter(EFilterMode filter);
		D3D12_TEXTURE_ADDRESS_MODE		TranslateAddressMode(ETextureAddressMode mode);
		D3D12_COMPARISON_FUNC			TranslateCompareFunction(ECompareFunction func);

		//const char*					TranslateVertexElementUsageSemantic(VertexElementUsage usage);
		D3D12_PRIMITIVE_TOPOLOGY		TranslatePrimitiveTopology(EMeshTopologyType type);
		D3D12_PRIMITIVE_TOPOLOGY_TYPE	TranslatePrimitiveTopologyType(EMeshTopologyType type);
		D3D12_CULL_MODE					TranslateCullMode(ECullMode CullMode);
		D3D12_FILL_MODE					TranslateFillMode(EFillMode FillMode);
		D3D12_STENCIL_OP				TranslateStencilOp(EStencilOperation StencilOp);
		D3D12_BLEND_OP					TranslateBlendOp(EBlendOperation BlendOp);
		D3D12_BLEND						TranslateBlendFactor(EBlendFactor BlendFactor);
		//void TranslateVertexStream(std::vector<D3D12_INPUT_ELEMENT_DESC>& elements, size_t stream_index,
		//	uint32_t buf_offset, bool is_instance, std::span<VertexStreamLayout const> vet);

		D3D12_HIT_GROUP_TYPE			TranslateHitGroupType(EHitGroupType type);

		PixelFormat						TranslateFromPlatformFormat(DXGI_FORMAT format);
		DXGI_FORMAT						TranslateToPlatformFormat(PixelFormat format);
		DXGI_FORMAT						TranslateToPlatformFormat(VertexFormat format);

		D3D_ROOT_SIGNATURE_VERSION		TranslateRootSignatureVersion(ERootSignatureVersion v);
	};

};


