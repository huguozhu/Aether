module Aether:Engine;
import :Engine;
import :EngineDefinition;
import :D3D12Context;
import :Error;
import :Job;
import <thread>;


namespace Aether
{
    AetherEngine::AetherEngine(EngineInitInfo& init_info)
    {
        m_InitInfo = init_info;
    }
    AResult AetherEngine::Initialize(void* device, void* native_wnd)
    {
        // RHI Context
        if (m_InitInfo.rhi_type == ERHIType::D3D12)
            m_pRHIContext = MakeSharedPtr<D3D12Context>(this);
        else if (m_InitInfo.rhi_type == ERHIType::Vulkan)
            ;//m_pRHIContext = MakeSharedPtr<VulkanContext>(this);

        // Job System
        m_pJobSystem = MakeSharedPtr<JobSystem>(1);
        m_pJobSystem->Initialize();

        // Thread Pool

        // Scene Manager
               

        return A_Success;
    }
    AResult AetherEngine::Tick()
    {
        return A_Success;
    }
    AResult AetherEngine::Update()
    {
        return A_Success;
    }
    AResult AetherEngine::BeginRender()
    {
        return A_Success;
    }
    AResult AetherEngine::RenderFrame()
    {
        return A_Success;
    }
    AResult AetherEngine::EndRender()
    {
        return A_Success;
    }



};