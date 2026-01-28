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

        RHIShaderPtr CreateShader(EShaderStage stage, std::string const& name, std::string const& entry_func_name, std::string const& code) override { return nullptr; }
        RHIMeshPtr CreateMesh() override { return nullptr; }

        RHIBufferPtr CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data = nullptr) { return nullptr; }
        RHIBufferPtr CreateVertexBuffer(uint32_t data_size, const void* data = nullptr) { return nullptr; }
        RHIBufferPtr CreateIndexBuffer(uint32_t data_size, const void* data = nullptr) { return nullptr; }


		void* GetVulkanInstance() { return m_pVkInstance; }
        void* GetPhysicalDevice() { return m_pVkPhysicalDevice; }

        AResult CheckCapabilitySetSupport() override { return A_Success; }

    private:
		AResult CreateVulkanInstance();

    private:
        void*              m_pVkInstance = nullptr;
        void*              m_pVkPhysicalDevice = nullptr;

        std::vector<std::string> supportedInstanceExtensions;
        std::vector<const char*> enabledInstanceExtensions;
    };

};