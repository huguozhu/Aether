module;
#include "vulkan/vulkan.h"
export module Aether:VulkanDefinition;
import std;

export namespace Aether
{
    export inline void VkThrowIfFailed(VkResult hr) {
        if (hr != VK_SUCCESS) {
            // 抛出异常或处理错误
            throw std::runtime_error("Vulkan operation failed");
        }
    }
}

