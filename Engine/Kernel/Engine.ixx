export module Aether:Engine;

import :EngineDefinition;
import :RHIStruct;
import :Semaphore;
import :Mutex;
import :Thread;
import <string>;

export namespace Aether
{
    enum class ERendererType : uint32_t
    {
        Unknown,
        Forward,
        Deferred,
    };

    enum class FPSLimitType : uint32_t
    {
        NoLImit = 1000,
        FPS_30 = 30,
        FPS_60 = 60,
        FPS_120 = 90,
    };
    enum class RenderFrameRetVal
    {
        RenderSuccess,
        Timeout,
        Exiting,
        Num,
    };

    struct EngineInitInfo
    {
        ERHIType            rhi_type = ERHIType::D3D12;
        ERendererType       renderer_type = ERendererType::Forward;
        bool                enable_debug = true;
        int                 adapter_index = 0;
        int                 max_worker_threads = -1;
        bool                enable_multi_thread = true;
        bool                is_full_screen = false;
        FPSLimitType        fps_limit_type = FPSLimitType::FPS_60;
    };

    class AetherEngine
    {
    public:
        AetherEngine() = default;
        AetherEngine(EngineInitInfo& init_info);
        AResult Initialize(void* device, void* native_wnd);
        AResult Uninitialize();

        AResult Tick();
        // Called By MainThread
        AResult Update();
        AResult BeginRender();
        AResult EndRender();
        // Called By RenderThread
        RenderFrameRetVal RenderFrame();


        ERHIType        GetRHIType()            { return m_InitInfo.rhi_type; }
        bool            EnableDebug()           { return m_InitInfo.enable_debug; }
        bool            EnableMultiThread()     { return m_InitInfo.enable_multi_thread; }
        bool            IsFullScreen()          { return m_InitInfo.is_full_screen; }
        int             GetPreferredAdapter()   { return m_InitInfo.adapter_index; }
        FPSLimitType    GetFpsLimitType()       { return m_InitInfo.fps_limit_type; }
        ERendererType   GetRendererType()       { return m_InitInfo.renderer_type; }

        void            SetFpsLimitType(FPSLimitType v);

        RHIContext&         RHIContextInstance()        { return *m_pRHIContext;}
        SceneManager&       SceneManagerInstance()      { return *m_pSceneManager; }
        SceneRenderer&      SceneRendererInstance()     { return *m_pSceneRenderer; }
        ResourceManager&    ResourceManagerInstance()   { return *m_pResourceManager; }
        Effect&             EffectInstance()            { return *(m_pEffect.get()); }

        uint32_t    GetFrameCount()     const { return m_FrameCount; }
        double      GetCurTime()        const { return m_dCurTime; }
        double      GetDeltaTime()      const { return m_dDeltaTime; } 

    private:
        AResult InitRHIContext();
        AResult DoRenderFrame();
        
    private:
        EngineInitInfo  m_InitInfo;

        RHIContextPtr           m_pRHIContext = nullptr;
        JobSystemPtr            m_pJobSystem = nullptr;
        SceneManagerPtr         m_pSceneManager = nullptr;
        SceneRendererPtr        m_pSceneRenderer = nullptr;
        ResourceManagerPtr      m_pResourceManager = nullptr;
        EffectPtrUnique         m_pEffect;

        bool m_bRunning = false;


        TimerPtrUnique  m_pTimer;
        double          m_dCurTime = 0.0;
        double          m_dDeltaTime = 0.0;
        double          m_fMinFrameTime = 0.0;
        uint32_t        m_FrameCount = 0;

        // Multi Threads
        Semaphore   m_MainSem;
        Semaphore   m_RenderSem;        
        Semaphore   m_EncoderEndSem;

        Mutex       m_EncoderApiLock;
        Mutex       m_ResourceApiLock;
        Thread      m_RenderThread;



    };

};