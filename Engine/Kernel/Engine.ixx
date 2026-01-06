export module Aether:Engine;

import :EngineDefinition;
import :RHIStruct;
import <string>;


export namespace Aether
{
    struct EngineInitInfo
    {
        ERHIType    rhi_type = ERHIType::D3D12;
        bool        enable_debug = true;
        int         adapter_index = 0;
        int         max_worker_threads = -1;
    };

    class AetherEngine
    {
    public:
        AetherEngine(EngineInitInfo& init_info);
        AResult Initialize(void* device, void* native_wnd);

        AResult Run();
        void MainLoop();
        AResult ShutdownEngine();

        ERHIType GetRHIType() { return m_InitInfo.rhi_type; }
        bool EnableDebug() { return m_InitInfo.enable_debug; }
        int GetPreferredAdapter() { return m_InitInfo.adapter_index; }




    private:
        EngineInitInfo  m_InitInfo;

        RHIContextPtr   m_pRHIContext = nullptr;
        JobSystemPtr    m_pJobSystem = nullptr;
        SceneManagerPtr m_pSceneManager = nullptr;

        bool m_bRunning = false;
    };

};