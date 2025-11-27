export module Aether.RHI:rhi_struct;

export namespace Aether
{
    enum class RHI_TYPE {
        DX12,
        VULKAN,        
    };

    enum class COMMAND_LIST_TYPE {
        GRAPHICS,
        COMPUTE,
        COPY,
        BUNDLE
    };

    enum class COMMAND_LIST_STATE {
        RECORDING,
        EXECUTABLE,
        PENDING_EXECUTION,
        INVALID
    };

};