module Aether:Engine;
import :Engine;
import :EngineDefinition;
import :D3D12Context;
import :Error;
import :Job;
import :Timer;
import :Log;
import :SceneManager;

import <thread>;


namespace Aether
{
    static AResult s_RenderThread(Thread* pThread, void* user_data)
    {
        AetherEngine* pEngine = (AetherEngine*)user_data;
        LOG_INFO("Render Thread Start... ...");
        while (RenderFrameRetVal::Exiting != pEngine->RenderFrame()) {}
        LOG_INFO("Render Thread Exit ... ...");
        return A_Success;
    }

    AetherEngine::AetherEngine(EngineInitInfo& init_info)
    {
        m_InitInfo = init_info;
        m_pTimer = MakeUniquePtr<Timer>();
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
        if (!m_pSceneManager)
        {
            m_pSceneManager = MakeSharedPtr<SceneManager>(this);
        }

        // Init Render Thread
        m_RenderThread.Init(s_RenderThread, this, 0, "RenderThread");

        this->SetFpsLimitType(m_InitInfo.fps_limit_type);
        return A_Success;
    }

    void AetherEngine::SetFpsLimitType(FPSLimitType v)
    {
        m_InitInfo.fps_limit_type = v;
        m_fMinFrameTime = 1.0f / (float)v;
    }

    AResult AetherEngine::Tick()
    {
        return A_Success;
    }
    AResult AetherEngine::Update()
    {
        double last_time = m_dCurTime;
        float cur_time = m_pTimer->CurrentTimeSinceEpoch_S();
        float time_interval = cur_time - last_time;
        float  limit_time = 0.0;
        if (time_interval < m_fMinFrameTime)
            return A_Success;
        m_dCurTime = cur_time;
        m_dDeltaTime = m_dCurTime - last_time;

        this->BeginRender();
        AETHER_RETIF_FAIL(SceneManagerInstance().Tick((float)m_dDeltaTime));
        this->EndRender();
        return A_Success;
    }
    AResult AetherEngine::BeginRender()
    {
        m_RenderSem.Signal();
        LOG_INFO("Main_Thread  :BeginRender()... ...");
        return A_Success;
    }
    RenderFrameRetVal AetherEngine::RenderFrame()
    {
        m_RenderSem.WaitForSignal();
        LOG_INFO("Render_Thread: ... ...");
        // add Rendering code ...

        m_MainSem.Signal();
        return RenderFrameRetVal::RenderSuccess;
    }
    AResult AetherEngine::EndRender()
    {
        m_MainSem.WaitForSignal();
        LOG_INFO("Main_Thread  :EndRender()... ...\n\n");

        return A_Success;
    }
};