module;
#include "vulkan/vulkan.h"

export module Aether:VulkanContext;
import :RHIContext;
import :Engine;
import :EngineDefinition;

export namespace Aether
{
    class VulkanContext : public RHIContext
    {
    public:
        VulkanContext(AetherEngine* engine);
        ~VulkanContext() = default;

        AResult Init() override;
        AResult AttachNativeWindows(std::string const& name, void* native_wnd) override { return A_Success; }
        AResult SwapBuffers() override { return A_Success; }

        RHIShaderPtr CreateShader(EShaderStage stage, std::string const& name, std::string const& entry_func_name, const void* byteCode, size_t byteCodeSize) override { return nullptr; }
        RHIMeshPtr CreateMesh() override { return nullptr; }

        RHIBufferPtr CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data = nullptr) { return nullptr; }
        RHIBufferPtr CreateVertexBuffer(uint32_t data_size, const void* data = nullptr) { return nullptr; }
        RHIBufferPtr CreateIndexBuffer(uint32_t data_size, const void* data = nullptr) { return nullptr; }


		void* GetVulkanInstance() { return m_pVkInstance; }
        void* GetPhysicalDevice() { return m_pVkPhysicalDevice; }

        AResult CheckCapabilitySetSupport() override { return A_Success; }

    private:
		AResult CreateVulkanInstance();
        AResult CreatePhysicalDevices();
		AResult CreateLogicalDevice(bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
		AResult CreateCommandPool(VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

    private:
        VkInstance          m_pVkInstance = nullptr;
        VkPhysicalDevice    m_pVkPhysicalDevice = nullptr;
        VkDevice	        m_pVkDevice = nullptr;
		VkCommandPool       m_pVkCommandPool = nullptr;


        std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
        std::vector<std::string> m_vSupportedInstanceExtensions;
        std::vector<const char*> m_vEnabledInstanceExtensions;
        VkPhysicalDeviceFeatures m_EnabledFeatures{};
    };

};