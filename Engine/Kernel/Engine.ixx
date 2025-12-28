export module Aether:Engine;

import :EngineDefinition;
import :RHIStruct;
import <string>;


export namespace Aether
{
    struct EngineInitInfo
    {
        ERHIType    rhi_type = ERHIType::D3D12;
    };

    class AetherEngine
    {
    public:
        AetherEngine(EngineInitInfo& init_info);
        AResult Initialize(void* device, void* native_wnd);

        AResult StartEngine();
        AResult ShutdownEngine();

        ERHIType GetRHIType() { return m_InitInfo.rhi_type; }

    private:
        EngineInitInfo  m_InitInfo;

        RHIContextPtr   m_pRHIContext = nullptr;
        SceneManagerPtr m_pSceneManager = nullptr;
        
    };
    
}