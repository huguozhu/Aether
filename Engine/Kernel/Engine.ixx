export module Aether.Kernel:Engine;

import :EngineDefinition;
import <string>;
import Aether.RHI;

export namespace Aether
{
    struct EngineInitInfo
    {
        ERHIType                 rhi_type = ERHIType::D3D12;
    };

    class AetherEngine
    {
    public:
        void StartEngine(EngineInitInfo& init_info);
        void ShutdownEngine();

        void Initialize();

        

    private:
        RHIContextPtr   m_pRHIContext = nullptr;
        SceneManagerPtr m_pSceneManager = nullptr;
    

        EngineInitInfo m_InitInfo;
    };

    
}