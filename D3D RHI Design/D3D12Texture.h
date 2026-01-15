class D3D12Texture : public ITexture {
private:
    ComPtr<ID3D12Resource> m_resource;
    ComPtr<ID3D12Resource> m_uploadResource; // 用于初始上传
    ComPtr<ID3D12Resource> m_readbackResource; // 用于回读
    
    TextureDesc m_desc;
    RESOURCE_STATE m_currentState;
    
    // 描述符句柄
    DescriptorHandle m_srvHandle;
    DescriptorHandle m_rtvHandle;
    DescriptorHandle m_dsvHandle;
    DescriptorHandle m_uavHandle;
    
    D3D12Device* m_device;

public:
    D3D12Texture(D3D12Device* device, const TextureDesc& desc, const void* initialData = nullptr);
    ~D3D12Texture() override;
    
    // IResource接口
    RESOURCE_STATE GetCurrentState() const override { return m_currentState; }
    void SetCurrentState(RESOURCE_STATE state) override { m_currentState = state; }
    size_t GetSize() const override;
    FORMAT GetFormat() const override { return m_desc.format; }
    const ResourceDesc& GetDesc() const override { return m_desc; }
    
    // ITexture接口
    TEXTURE_TYPE GetType() const override { return m_desc.type; }
    uint32_t GetWidth() const override { return m_desc.width; }
    uint32_t GetHeight() const override { return m_desc.height; }
    uint32_t GetDepth() const override { return m_desc.depth; }
    uint32_t GetMipLevels() const override { return m_desc.mipLevels; }
    uint32_t GetArraySize() const override { return m_desc.arraySize; }
    uint32_t GetSampleCount() const override { return m_desc.sampleCount; }
    uint32_t GetSampleQuality() const override { return m_desc.sampleQuality; }
    
    void* Map(uint32_t subresource = 0) override;
    void Unmap(uint32_t subresource = 0) override;
    
    // D3D12特定方法
    ID3D12Resource* GetNativeResource() { return m_resource.Get(); }
    
    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const { 
        return { static_cast<SIZE_T>(m_rtvHandle.cpuHandle) };
    }
    
    D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const { 
        return { static_cast<SIZE_T>(m_dsvHandle.cpuHandle) };
    }
    
    D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() const { 
        return { static_cast<SIZE_T>(m_srvHandle.cpuHandle) };
    }
    
    D3D12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() const { 
        return { static_cast<SIZE_T>(m_uavHandle.cpuHandle) };
    }
    
    void TransitionTo(ICommandList* cmdList, RESOURCE_STATE newState);
    
private:
    void CreateResource(const void* initialData);
    void CreateViews();
    DXGI_FORMAT GetDXGIFormat() const;
    D3D12_RESOURCE_FLAGS GetResourceFlags() const;
    D3D12_RESOURCE_DIMENSION GetResourceDimension() const;
};

D3D12Texture::D3D12Texture(D3D12Device* device, const TextureDesc& desc, const void* initialData)
    : m_device(device)
    , m_desc(desc)
    , m_currentState(desc.initialState) {
    
    CreateResource(initialData);
    
    if (initialData) {
        // 如果有初始数据，进行上传
        UploadInitialData(initialData);
    }
    
    CreateViews();
}

void D3D12Texture::CreateResource(const void* initialData) {
    auto d3dDevice = m_device->GetNativeDevice();
    
    D3D12_RESOURCE_DESC resourceDesc = {};
    resourceDesc.Dimension = GetResourceDimension();
    resourceDesc.Alignment = 0;
    resourceDesc.Width = m_desc.width;
    resourceDesc.Height = m_desc.height;
    resourceDesc.DepthOrArraySize = (resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D) 
        ? m_desc.depth : m_desc.arraySize;
    resourceDesc.MipLevels = m_desc.mipLevels;
    resourceDesc.Format = GetDXGIFormat();
    resourceDesc.SampleDesc.Count = m_desc.sampleCount;
    resourceDesc.SampleDesc.Quality = m_desc.sampleQuality;
    resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    resourceDesc.Flags = GetResourceFlags();
    
    D3D12_HEAP_PROPERTIES heapProperties = {};
    heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    heapProperties.CreationNodeMask = 1;
    heapProperties.VisibleNodeMask = 1;
    
    D3D12_RESOURCE_STATES initialState = ConvertResourceState(m_desc.initialState);
    
    D3D12_CLEAR_VALUE clearValue = {};
    if (m_desc.optimizedClearValue.has_value()) {
        clearValue.Format = resourceDesc.Format;
        if (IsDepthFormat(m_desc.format)) {
            clearValue.DepthStencil.Depth = m_desc.optimizedClearValue->depthStencil.depth;
            clearValue.DepthStencil.Stencil = m_desc.optimizedClearValue->depthStencil.stencil;
        } else {
            memcpy(clearValue.Color, m_desc.optimizedClearValue->color, sizeof(float) * 4);
        }
    }
    
    D3D12_CLEAR_VALUE* pClearValue = nullptr;
    if (m_desc.optimizedClearValue.has_value()) {
        pClearValue = &clearValue;
    }
    
    ThrowIfFailed(d3dDevice->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        initialState,
        pClearValue,
        IID_PPV_ARGS(&m_resource)
    ));
    
    // 设置资源名称（调试用）
    if (!m_desc.name.empty()) {
        m_resource->SetName(StringToWString(m_desc.name).c_str());
    }
}

void D3D12Texture::CreateViews() {
    auto descriptorManager = m_device->GetDescriptorManager();
    
    // 根据需要创建视图
    if (m_desc.bindFlags & BIND_SHADER_RESOURCE) {
        m_srvHandle = descriptorManager->AllocateSRV(this);
    }
    
    if (m_desc.bindFlags & BIND_RENDER_TARGET) {
        m_rtvHandle = descriptorManager->AllocateRTV(this);
    }
    
    if (m_desc.bindFlags & BIND_DEPTH_STENCIL) {
        m_dsvHandle = descriptorManager->AllocateDSV(this);
    }
    
    if (m_desc.bindFlags & BIND_UNORDERED_ACCESS) {
        m_uavHandle = descriptorManager->AllocateUAV(this);
    }
}

DXGI_FORMAT D3D12Texture::GetDXGIFormat() const {
    static const std::unordered_map<FORMAT, DXGI_FORMAT> formatMap = {
        {FORMAT::R8G8B8A8_UNORM, DXGI_FORMAT_R8G8B8A8_UNORM},
        {FORMAT::R8G8B8A8_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB},
        {FORMAT::R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT},
        {FORMAT::D32_FLOAT, DXGI_FORMAT_D32_FLOAT},
        {FORMAT::D24_UNORM_S8_UINT, DXGI_FORMAT_D24_UNORM_S8_UINT},
        // ... 其他格式映射
    };
    
    auto it = formatMap.find(m_desc.format);
    return it != formatMap.end() ? it->second : DXGI_FORMAT_UNKNOWN;
}