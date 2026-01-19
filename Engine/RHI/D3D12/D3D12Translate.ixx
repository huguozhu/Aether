export module Aether:D3D12Translate;
import :RHIStruct;
import :D3D12Definition;
import :RHIRootSignature;

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

	};

};


