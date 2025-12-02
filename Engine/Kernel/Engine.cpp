module Aether.Kernel:Engine;
import :Engine;
import :EngineDefinition;
import Aether.RHI;


namespace Aether
{
    void AetherEngine::StartEngine(EngineInitInfo& init_info)
    {
        m_InitInfo = init_info;
        if (init_info.rhi_type == ERHIType::D3D12)
        {
            // 初始化 D3D12 RHI 上下文
            //m_pRHIContext = MakeSharedPtr<D3D12Context>();            
        }
        else
        {
            // 其他 RHI 类型初始化
        }
       
    }

    void AetherEngine::ShutdownEngine()
    {

    }
    void AetherEngine::Initialize()
    {

    }



};