module Aether:Engine;
import :Engine;
import :EngineDefinition;
import :D3D12Context;
import :error;
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
        

        // Thread Pool

        // Scene Manager
               

        return A_Success;
    }
    AResult AetherEngine::Run()
    {
        m_bRunning = true;
        std::thread main_thread(&AetherEngine::MainLoop, this);
        return A_Success;
    }
    AResult AetherEngine::ShutdownEngine()
    {
        return A_Success;
    }
    void AetherEngine::MainLoop()
    {
        while (m_bRunning)
        {
            // Step1: 处理输入

            // Step2: 处理逻辑层更新

            // Step3: 开始渲染帧
            //Job* render_prepare_job = m_job_system->create_job([this, &logic_frame]() {
            //    // 这里可以并行执行：
            //    // 1. 视锥体剔除
            //    // 2. 材质排序
            //    // 3. 命令缓冲区生成
            //    prepare_render_data_parallel(logic_frame);
            //    });
            //m_job_system->run(render_prepare_job);

            // 等待准备完成并执行渲染
            //m_job_system->wait(render_prepare_job);
            //m_render_system->render_frame();

            // 结束帧并呈现
            //m_render_system->end_frame();

            // 帧同步（如果启用垂直同步或帧率限制）
            //frame_synchronization(delta_time);

            // 交换双缓冲索引
            //m_current_frame_index = (m_current_frame_index + 1) % 2;
        }
        return;
    }


};