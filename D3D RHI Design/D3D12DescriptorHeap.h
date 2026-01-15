class D3D12DescriptorHeap : public IDescriptorHeap {
private:
    ComPtr<ID3D12DescriptorHeap> m_descriptorHeap;
    DescriptorHeapDesc m_desc;
    
    uint32_t m_descriptorSize;
    uint32_t m_currentIndex;
    std::vector<bool> m_allocatedDescriptors;
    
    D3D12Device* m_device;
    D3D12_CPU_DESCRIPTOR_HANDLE m_cpuStart;
    D3D12_GPU_DESCRIPTOR_HANDLE m_gpuStart;

public:
    D3D12DescriptorHeap(D3D12Device* device, const DescriptorHeapDesc& desc);
    ~D3D12DescriptorHeap() override;
    
    DESCRIPTOR_HEAP_TYPE GetType() const override { return m_desc.type; }
    uint32_t GetCapacity() const override { return m_desc.capacity; }
    uint32_t GetSize() const override { return m_currentIndex; }
    
    DescriptorHandle Allocate(uint32_t count = 1) override;
    void Free(const DescriptorHandle& handle, uint32_t count = 1) override;
    
    void* GetCPUHandle(uint32_t index) const override;
    void* GetGPUHandle(uint32_t index) const override;
    void* GetNativeHeap() override { return m_descriptorHeap.Get(); }
    
    D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CPUHandle(uint32_t index) const;
    D3D12_GPU_DESCRIPTOR_HANDLE GetD3D12GPUHandle(uint32_t index) const;

private:
    D3D12_DESCRIPTOR_HEAP_TYPE ConvertHeapType(DESCRIPTOR_HEAP_TYPE type);
    D3D12_DESCRIPTOR_HEAP_FLAGS ConvertHeapFlags(DESCRIPTOR_HEAP_FLAGS flags);
};

D3D12DescriptorHeap::D3D12DescriptorHeap(D3D12Device* device, const DescriptorHeapDesc& desc)
    : m_device(device)
    , m_desc(desc)
    , m_currentIndex(0) {
    
    auto d3dDevice = m_device->GetNativeDevice();
    
    D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
    heapDesc.Type = ConvertHeapType(desc.type);
    heapDesc.NumDescriptors = desc.capacity;
    heapDesc.Flags = ConvertHeapFlags(desc.flags);
    heapDesc.NodeMask = 0;
    
    ThrowIfFailed(d3dDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_descriptorHeap)));
    
    // 获取描述符大小
    m_descriptorSize = d3dDevice->GetDescriptorHandleIncrementSize(heapDesc.Type);
    
    // 获取起始句柄
    m_cpuStart = m_descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    if (desc.flags & DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) {
        m_gpuStart = m_descriptorHeap->GetGPUDescriptorHandleForHeapStart();
    }
    
    // 初始化分配表
    m_allocatedDescriptors.resize(desc.capacity, false);
    
    // 设置名称
    if (!desc.name.empty()) {
        m_descriptorHeap->SetName(StringToWString(desc.name).c_str());
    }
}

DescriptorHandle D3D12DescriptorHeap::Allocate(uint32_t count) {
    if (count == 0 || count > m_desc.capacity) {
        return DescriptorHandle();
    }
    
    // 查找连续的空闲描述符
    uint32_t startIndex = 0;
    uint32_t freeCount = 0;
    
    for (uint32_t i = 0; i < m_desc.capacity; i++) {
        if (!m_allocatedDescriptors[i]) {
            if (freeCount == 0) {
                startIndex = i;
            }
            freeCount++;
            
            if (freeCount == count) {
                // 找到足够的连续空间
                break;
            }
        } else {
            freeCount = 0;
        }
    }
    
    if (freeCount < count) {
        // 没有足够的连续空间
        LOG_ERROR("Not enough continuous descriptors in heap");
        return DescriptorHandle();
    }
    
    // 标记为已分配
    for (uint32_t i = 0; i < count; i++) {
        m_allocatedDescriptors[startIndex + i] = true;
    }
    
    m_currentIndex = std::max(m_currentIndex, startIndex + count);
    
    // 创建句柄
    DescriptorHandle handle;
    handle.index = startIndex;
    handle.cpuHandle = reinterpret_cast<void*>(
        m_cpuStart.ptr + startIndex * m_descriptorSize);
    
    if (m_desc.flags & DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) {
        handle.gpuHandle = reinterpret_cast<void*>(
            m_gpuStart.ptr + startIndex * m_descriptorSize);
    }
    
    return handle;
}

void D3D12DescriptorHeap::Free(const DescriptorHandle& handle, uint32_t count) {
    if (!handle.IsValid() || handle.index >= m_desc.capacity) {
        return;
    }
    
    // 标记为未分配
    uint32_t endIndex = std::min(handle.index + count, m_desc.capacity);
    for (uint32_t i = handle.index; i < endIndex; i++) {
        m_allocatedDescriptors[i] = false;
    }
}