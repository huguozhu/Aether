class D3D12CommandQueue : public ICommandQueue {
private:
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12Fence> m_fence;
    HANDLE m_fenceEvent;
    uint64_t m_fenceValue;
    std::mutex m_queueMutex;
    
    D3D12_COMMAND_LIST_TYPE m_queueType;

public:
    D3D12CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE queueType);
    ~D3D12CommandQueue();

    void ExecuteCommandLists(uint32_t count, ICommandList** lists) override;
    void Signal(IFence* fence, uint64_t value) override;
    void Wait(IFence* fence, uint64_t value) override;
    
    // D3D12特定方法
    ID3D12CommandQueue* GetNativeQueue() const { return m_commandQueue.Get(); }
    uint64_t Signal();
    void WaitForFenceValue(uint64_t fenceValue);
    bool IsFenceComplete(uint64_t fenceValue);
    
    D3D12_COMMAND_LIST_TYPE GetQueueType() const { return m_queueType; }

private:
    void Initialize(ID3D12Device* device);
};


D3D12CommandQueue::D3D12CommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE queueType)
    : m_queueType(queueType)
    , m_fenceValue(0)
{
    Initialize(device);
}

void D3D12CommandQueue::Initialize(ID3D12Device* device) {
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = m_queueType;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;
    
    ThrowIfFailed(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue)));
    ThrowIfFailed(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));
    
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    if (m_fenceEvent == nullptr) {
        ThrowIfFailed(HRESULT_FROM_WIN32(GetLastError()));
    }
}

D3D12CommandQueue::~D3D12CommandQueue() {
    if (m_fenceEvent != nullptr) {
        CloseHandle(m_fenceEvent);
        m_fenceEvent = nullptr;
    }
}

void D3D12CommandQueue::ExecuteCommandLists(uint32_t count, ICommandList** lists) {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    
    std::vector<ID3D12CommandList*> d3dCommandLists;
    d3dCommandLists.reserve(count);
    
    for (uint32_t i = 0; i < count; ++i) {
        auto d3d12List = static_cast<D3D12CommandList*>(lists[i]);
        if (d3d12List->GetState() != COMMAND_LIST_STATE::EXECUTABLE) {
            throw std::runtime_error("Command list must be in executable state");
        }
        
        d3dCommandLists.push_back(d3d12List->GetNativeCommandList());
    }
    
    m_commandQueue->ExecuteCommandLists(count, d3dCommandLists.data());
    
    // 更新命令列表状态
    for (uint32_t i = 0; i < count; ++i) {
        auto d3d12List = static_cast<D3D12CommandList*>(lists[i]);
        // 状态变为等待执行完成
    }
}

void D3D12CommandQueue::Signal(IFence* fence, uint64_t value) {
    auto d3d12Fence = static_cast<D3D12Fence*>(fence);
    ThrowIfFailed(m_commandQueue->Signal(d3d12Fence->GetNativeFence(), value));
}

void D3D12CommandQueue::Wait(IFence* fence, uint64_t value) {
    auto d3d12Fence = static_cast<D3D12Fence*>(fence);
    ThrowIfFailed(m_commandQueue->Wait(d3d12Fence->GetNativeFence(), value));
}

uint64_t D3D12CommandQueue::Signal() {
    uint64_t fenceValue = ++m_fenceValue;
    ThrowIfFailed(m_commandQueue->Signal(m_fence.Get(), fenceValue));
    return fenceValue;
}

void D3D12CommandQueue::WaitForFenceValue(uint64_t fenceValue) {
    if (!IsFenceComplete(fenceValue)) {
        ThrowIfFailed(m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent));
        WaitForSingleObject(m_fenceEvent, INFINITE);
    }
}

bool D3D12CommandQueue::IsFenceComplete(uint64_t fenceValue) {
    return m_fence->GetCompletedValue() >= fenceValue;
}