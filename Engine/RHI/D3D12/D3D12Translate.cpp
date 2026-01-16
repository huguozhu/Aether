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

	};
};