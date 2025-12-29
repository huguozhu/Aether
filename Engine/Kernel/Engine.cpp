module Aether:Engine;
import :Engine;
import :EngineDefinition;
import :D3D12Context;
import :error;


namespace Aether
{
    AetherEngine::AetherEngine(EngineInitInfo& init_info)
    {
        m_InitInfo = init_info;
    }
    AResult AetherEngine::Initialize(void* device, void* native_wnd)
    {
        if (m_InitInfo.rhi_type == ERHIType::D3D12)
            m_pRHIContext = MakeSharedPtr<D3D12Context>(this);
        else if (m_InitInfo.rhi_type == ERHIType::Vulkan)
            ;//m_pRHIContext = MakeSharedPtr<VulkanContext>(this);

        return A_Success;
    }
    AResult AetherEngine::StartEngine()
    {
        return A_Success;
    }
    AResult AetherEngine::ShutdownEngine()
    {
        return A_Success;
    }
    



};