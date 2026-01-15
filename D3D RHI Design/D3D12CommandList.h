class D3D12CommandList : public ICommandList {
private:
    ComPtr<ID3D12GraphicsCommandList> m_commandList;
    ComPtr<ID3D12CommandAllocator> m_allocator;
    COMMAND_LIST_STATE m_state;
    COMMAND_LIST_TYPE m_type;
    
    // 当前绑定状态
    ID3D12PipelineState* m_currentPSO;
    ID3D12RootSignature* m_currentRootSignature;
    D3D12_VIEWPORT m_viewport;
    D3D12_RECT m_scissorRect;
    
    // 资源追踪
    std::vector<ComPtr<ID3D12Resource>> m_trackedResources;

public:
    D3D12CommandList(ID3D12Device* device, COMMAND_LIST_TYPE type);
    ~D3D12CommandList();

    // 生命周期管理
    void Begin() override;
    void End() override;
    void Reset() override;
    
    // 资源绑定
    void SetPipelineState(IPipelineState* pipeline) override;
    void SetVertexBuffers(uint32_t startSlot, uint32_t count, IBuffer** buffers) override;
    void SetIndexBuffer(IBuffer* buffer) override;
    void SetDescriptorSets(uint32_t setCount, IDescriptorSet** sets) override;
    
    // 绘制命令
    void Draw(uint32_t vertexCount, uint32_t instanceCount = 1) override;
    void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1) override;
    
    // 计算命令
    void Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) override;
    
    // 资源屏障
    void ResourceBarrier(uint32_t barrierCount, const ResourceBarrier* barriers) override;
    
    // 查询状态
    COMMAND_LIST_STATE GetState() const override { return m_state; }
    COMMAND_LIST_TYPE GetType() const override { return m_type; }
    
    // D3D12特定方法
    ID3D12GraphicsCommandList* GetNativeCommandList() const { return m_commandList.Get(); }
    void TrackResource(ComPtr<ID3D12Resource> resource);
    
private:
    D3D12_COMMAND_LIST_TYPE ConvertCommandListType(COMMAND_LIST_TYPE type);
    void TransitionResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
};


D3D12CommandList::D3D12CommandList(ID3D12Device* device, COMMAND_LIST_TYPE type) 
    : m_state(COMMAND_LIST_STATE::INVALID)
    , m_type(type)
    , m_currentPSO(nullptr)
    , m_currentRootSignature(nullptr)
{
    D3D12_COMMAND_LIST_TYPE d3dType = ConvertCommandListType(type);
    
    // 创建命令分配器
    ThrowIfFailed(device->CreateCommandAllocator(
        d3dType, IID_PPV_ARGS(&m_allocator)));
    
    // 创建命令列表
    ThrowIfFailed(device->CreateCommandList(
        0, // node mask
        d3dType,
        m_allocator.Get(),
        nullptr, // initial PSO
        IID_PPV_ARGS(&m_commandList)));
    
    // 初始状态为关闭，符合D3D12规范
    m_commandList->Close();
    m_state = COMMAND_LIST_STATE::INVALID;
}

void D3D12CommandList::Begin() {
    if (m_state != COMMAND_LIST_STATE::INVALID) {
        throw std::runtime_error("Command list must be in invalid state to begin");
    }
    
    ThrowIfFailed(m_allocator->Reset());
    ThrowIfFailed(m_commandList->Reset(m_allocator.Get(), nullptr));
    
    m_state = COMMAND_LIST_STATE::RECORDING;
    
    // 重置当前状态
    m_currentPSO = nullptr;
    m_currentRootSignature = nullptr;
}

void D3D12CommandList::End() {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state to end");
    }
    
    ThrowIfFailed(m_commandList->Close());
    m_state = COMMAND_LIST_STATE::EXECUTABLE;
}

void D3D12CommandList::Reset() {
    if (m_state == COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Cannot reset command list while recording");
    }
    
    // 释放追踪的资源引用
    m_trackedResources.clear();
    m_state = COMMAND_LIST_STATE::INVALID;
}

void D3D12CommandList::SetPipelineState(IPipelineState* pipeline) {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state");
    }
    
    auto d3d12Pipeline = static_cast<D3D12PipelineState*>(pipeline);
    m_currentPSO = d3d12Pipeline->GetNativePSO();
    m_currentRootSignature = d3d12Pipeline->GetRootSignature();
    
    m_commandList->SetPipelineState(m_currentPSO);
    m_commandList->SetGraphicsRootSignature(m_currentRootSignature);
}

void D3D12CommandList::SetVertexBuffers(uint32_t startSlot, uint32_t count, IBuffer** buffers) {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state");
    }
    
    std::vector<D3D12_VERTEX_BUFFER_VIEW> views;
    views.reserve(count);
    
    for (uint32_t i = 0; i < count; ++i) {
        auto d3d12Buffer = static_cast<D3D12Buffer*>(buffers[i]);
        views.push_back(d3d12Buffer->GetVertexBufferView());
        
        // 追踪资源以确保生命周期
        TrackResource(d3d12Buffer->GetNativeResource());
    }
    
    m_commandList->IASetVertexBuffers(startSlot, count, views.data());
}

void D3D12CommandList::Draw(uint32_t vertexCount, uint32_t instanceCount) {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state");
    }
    
    m_commandList->DrawInstanced(vertexCount, instanceCount, 0, 0);
}

void D3D12CommandList::DrawIndexed(uint32_t indexCount, uint32_t instanceCount) {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state");
    }
    
    m_commandList->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

void D3D12CommandList::Dispatch(uint32_t groupCountX, uint32_t groupCountY, uint32_t groupCountZ) {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state");
    }
    
    m_commandList->Dispatch(groupCountX, groupCountY, groupCountZ);
}

void D3D12CommandList::ResourceBarrier(uint32_t barrierCount, const ResourceBarrier* barriers) {
    if (m_state != COMMAND_LIST_STATE::RECORDING) {
        throw std::runtime_error("Command list must be in recording state");
    }
    
    std::vector<D3D12_RESOURCE_BARRIER> d3dBarriers;
    d3dBarriers.reserve(barrierCount);
    
    for (uint32_t i = 0; i < barrierCount; ++i) {
        const auto& barrier = barriers[i];
        
        if (barrier.type == ResourceBarrier::Type::TRANSITION) {
            auto d3d12Resource = static_cast<D3D12Resource*>(barrier.resource)->GetNativeResource();
            
            D3D12_RESOURCE_BARRIER resourceBarrier = {};
            resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            resourceBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            resourceBarrier.Transition.pResource = d3d12Resource;
            resourceBarrier.Transition.StateBefore = ConvertResourceState(barrier.transition.stateBefore);
            resourceBarrier.Transition.StateAfter = ConvertResourceState(barrier.transition.stateAfter);
            resourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            
            d3dBarriers.push_back(resourceBarrier);
            
            // 追踪资源
            TrackResource(d3d12Resource);
        }
    }
    
    if (!d3dBarriers.empty()) {
        m_commandList->ResourceBarrier(static_cast<UINT>(d3dBarriers.size()), d3dBarriers.data());
    }
}

void D3D12CommandList::TrackResource(ComPtr<ID3D12Resource> resource) {
    m_trackedResources.push_back(resource);
}

D3D12_COMMAND_LIST_TYPE D3D12CommandList::ConvertCommandListType(COMMAND_LIST_TYPE type) {
    switch (type) {
    case COMMAND_LIST_TYPE::GRAPHICS:
        return D3D12_COMMAND_LIST_TYPE_DIRECT;
    case COMMAND_LIST_TYPE::COMPUTE:
        return D3D12_COMMAND_LIST_TYPE_COMPUTE;
    case COMMAND_LIST_TYPE::COPY:
        return D3D12_COMMAND_LIST_TYPE_COPY;
    case COMMAND_LIST_TYPE::BUNDLE:
        return D3D12_COMMAND_LIST_TYPE_BUNDLE;
    default:
        throw std::runtime_error("Unknown command list type");
    }
}