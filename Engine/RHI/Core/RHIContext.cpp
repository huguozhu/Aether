module Aether.RHI:RHIContext;
import :RHIContext;


namespace Aether
{
    RHIContext::RHIContext()
    {
    }
    //设置管线（带状态缓存优化）
    //void RHIContext::SetGraphicsPipeline(RHIPipelineState* pso) 
    //{
    //    if (currentState.pipelineState == pso) return;

    //    /commandList->SetPipeline(pso->GetNativeHandle());
    //    currentState.pipelineState = pso;
    //}
    ////自动资源屏障插入
    //void RHIContext::TransitionResource(RHIResource* res, EResourceState newState) 
    //{
    //    auto oldState = stateTracker.GetResourceState(res);
    //    if (oldState != newState) {
    //        EnsureResourceState(res, newState);  // 插入 Barrier
    //    }
    //}

    //void RHIContext::EnsureResourceState(RHIResource* res, EResourceState wanted)
    //{
    //    auto current = stateTracker.GetResourceState(res);
    //    if (current != wanted) {
    //        commandList->TransitionResource(res, current, wanted);
    //        stateTracker.SetResourceState(res, wanted);
    //    }
    //}

};