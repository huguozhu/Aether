export module Aether.RHI:RHIContext;

import :RHIStruct;
import :RHIDevice;
import :RHICommandList;
import :RHIResource;
import <string>;
import <vector>;

export namespace Aether
{
    class RHIContext
    {
    public:
        RHIContext();
        virtual ~RHIContext() = default;

        // --- 渲染目标控制 ---
    //    virtual void SetRenderTargets(
    //        uint32_t numRTVs,
    //        RHIResource** rtvs,
    //        RHIResource* dsv = nullptr
    //    );

    //    virtual void ClearColor(RHIResource* tex, const float rgba[4]);
    //    virtual void ClearDepth(RHIResource* tex, float depth = 1.0f);

    //    // --- 管线与着色器 ---
    //    virtual void SetGraphicsPipeline(RHIPipelineState* pso);
    //    virtual void SetComputePipeline(RHIPipelineState* pso);
    //    virtual void SetRayTracingPipeline(RHIPipelineState* pso);

    //    // --- 资源绑定 ---
    //    virtual void SetShaderResources(uint32_t startSlot, RHIResource** resources, uint32_t count);
    //    virtual void SetConstantBuffers(uint32_t slot, RHIBuffer** buffers, uint32_t count);
    //    virtual void SetSamplers(uint32_t slot, void** samplers, uint32_t count);

    //    // --- 绘制命令 ---
    //    virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1);
    //    virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1);
    //    virtual void DrawInstancedIndirect(RHIBuffer* argsBuffer, uint32_t offset = 0);

    //    virtual void Dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ);
    //    virtual void DispatchMesh(uint32_t x, uint32_t y, uint32_t z);

    //    virtual void TraceRays(RHIPipelineState* rtPSO, uint32_t width, uint32_t height);

    //    // --- 资源操作 ---
    //    virtual void CopyBuffer(RHIBuffer* dst, RHIBuffer* src);
    //    virtual void CopyTexture(RHITexture* dst, RHITexture* src);

    //    virtual void TransitionResource(RHIResource* res, EResourceState newState);
    //    virtual void UAVBarrier(RHIResource* uav = nullptr);  // 等待 UAV 写入完成

    //    // --- 提交与同步 ---
    //    virtual void Flush(bool bWait = false);       // 提交当前命令
    //    virtual void BeginEvent(const char* name);   // 调试标记（PIX / RenderDoc）
    //    virtual void EndEvent();

    //    // 获取底层命令列表（用于高级操作）
    //    RHICommandList* GetCommandList() const { return m_pCommandList; }

    //protected:
    //    // 内部辅助函数：确保资源处于正确状态
    //    void EnsureResourceState(RHIResource* res, EResourceState wanted);

    protected:
        RHIDevice* m_pDevice;
        RHICommandList* m_pCommandList;

        // 当前渲染状态缓存（用于减少重复设置）
        struct {
            void* pipelineState = nullptr;
            std::vector<RHIResource*> shaderResources[8];     // 各槽位资源
            std::vector<RHIBuffer*> constantBuffers[16];
            std::vector<void*> samplers[16];
            EResourceState renderTargetState = EResourceState::RenderTarget;
            EResourceState depthStencilState = EResourceState::DepthWrite;
        } currentState;

        // 资源状态追踪器（D3D12/Vulkan 需要显式管理）
        //RHIResourceStateTracker stateTracker;
    };

};