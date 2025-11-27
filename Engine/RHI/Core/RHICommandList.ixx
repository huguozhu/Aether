export module Aether.RHI:rhi_command_list;

import :rhi_struct;

import <string>;

export namespace Aether
{

    export class IPipelineState;
    export class IBuffer;
    export class IDescriptorSet;
    export class ICommandList;
    export class IFence;
    export class IResourceBarrier;


    // 命令列表基类
    class ICommandList {
    public:
        virtual ~ICommandList() = default;

        // 生命周期管理
        virtual void Begin() = 0;
        virtual void End() = 0;
        virtual void Reset() = 0;

        // 资源绑定
        virtual void SetPipelineState(IPipelineState* pipeline) = 0;
        virtual void SetVertexBuffers(uint32_t startSlot, uint32_t count, IBuffer** buffers) = 0;
        virtual void SetIndexBuffer(IBuffer* buffer) = 0;
        virtual void SetDescriptorSets(uint32_t setCount, IDescriptorSet** sets) = 0;

        // 绘制命令
        virtual void Draw(uint32_t vertexCount, uint32_t instanceCount = 1) = 0;
        virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) = 0;

        // 计算命令
        virtual void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) = 0;

        // 资源屏障
        virtual void ResourceBarrier(uint32_t barrierCount, const IResourceBarrier* barriers) = 0;

        // 查询状态
        virtual COMMAND_LIST_STATE GetState() const = 0;
        virtual COMMAND_LIST_TYPE GetType() const = 0;
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
        virtual void ExecuteCommandLists(uint32_t count, ICommandList** lists) = 0;
        virtual void Signal(IFence* fence, uint64_t value) = 0;
        virtual void Wait(IFence* fence, uint64_t value) = 0;
    };

};