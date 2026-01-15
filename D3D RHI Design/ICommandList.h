class ICommandList
{
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
    virtual void ResourceBarrier(uint32_t barrierCount, const ResourceBarrier* barriers) = 0;
    
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





class ICommandList : public ICommandBuffer {
public:
    // 渲染目标和视口
    virtual void SetRenderTargets(uint32_t count, ITexture** renderTargets, ITexture* depthStencil = nullptr) = 0;
    virtual void SetViewport(const Viewport& viewport) = 0;
    virtual void SetScissorRect(const Rect& rect) = 0;
    
    // 资源清除
    virtual void ClearRenderTarget(ITexture* renderTarget, const float color[4]) = 0;
    virtual void ClearDepthStencil(ITexture* depthStencil, float depth = 1.0f, uint8_t stencil = 0) = 0;
    
    // 资源更新
    virtual void UpdateBuffer(IBuffer* buffer, const void* data, size_t size, size_t offset = 0) = 0;
    virtual void CopyBuffer(IBuffer* srcBuffer, IBuffer* dstBuffer) = 0;
    virtual void CopyTexture(ITexture* srcTexture, ITexture* dstTexture) = 0;
    
    // 管线屏障
    virtual void UAVBarrier(IResource* resource = nullptr) = 0;
    virtual void AliasingBarrier(IResource* before, IResource* after) = 0;
    
    // 查询相关
    virtual void BeginQuery(IQueryHeap* queryHeap, QUERY_TYPE type, uint32_t index) = 0;
    virtual void EndQuery(IQueryHeap* queryHeap, QUERY_TYPE type, uint32_t index) = 0;
    virtual void ResolveQueryData(IQueryHeap* queryHeap, QUERY_TYPE type, 
                                 uint32_t startIndex, uint32_t numQueries, 
                                 IBuffer* destinationBuffer, uint64_t alignedDestinationBufferOffset) = 0;
};