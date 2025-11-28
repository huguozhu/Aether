export module Aether.RHI:RHICommandList;

import :RHIStruct;

import <string>;

export namespace Aether
{
    class RHIPipelineState;
    class RHIBuffer;
    class RHIDescriptorSet;
    class RHIFence;
    class RHIResourceBarrier;

    // 命令列表基类
    class RHICommandList {
    public:
        virtual ~RHICommandList() = default;

        // 生命周期管理
        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Reset() = 0;

        // 资源绑定
        virtual void SetPipelineState(RHIPipelineState* pipeline) = 0;
        virtual void SetVertexBuffers(uint32_t startSlot, uint32_t count, RHIBuffer** buffers) = 0;
        virtual void SetIndexBuffer(RHIBuffer* buffer) = 0;
        virtual void SetDescriptorSets(uint32_t setCount, RHIDescriptorSet** sets) = 0;

        // 绘制命令
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1) = 0;
        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;

        // 网格着色器绘制命令
        virtual void DispatchMesh(uint32_t x, uint32_t y, uint32_t z) = 0;

        // 计算命令
        virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;

        // 光线追踪命令
        virtual void TraceRays(void* rtPSO, uint32_t width, uint32_t height) = 0;

        // 资源屏障
        virtual void ResourceBarrier(uint32_t barrierCount, const RHIResourceBarrier* barriers) = 0;

        // 查询状态
        virtual ECommandListType GetState() const = 0;
        virtual ECommandListType GetType() const = 0;

        // 资源操作
        virtual void TransitionResource(void* resource, EResourceState oldState, EResourceState newState) = 0;
        virtual void CopyBuffer(void* dst, void* src) = 0;
    };

    // 命令分配器
    class ICommandAllocator {
    public:
        virtual ~ICommandAllocator() = default;
        virtual void Reset() = 0;
    };

    // 命令队列
    class ICommandQueue {
    public:
        virtual ~ICommandQueue() = default;
        virtual void ExecuteCommandLists(uint32_t count, RHICommandList** lists) = 0;
        virtual void Signal(RHIFence* fence, uint64_t value) = 0;
        virtual void Wait(RHIFence* fence, uint64_t value) = 0;
    };

};