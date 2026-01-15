class D3D12Device : public IDevice {
private:
    ComPtr<ID3D12Device> m_device;
    ComPtr<IDXGIFactory4> m_dxgiFactory;
    ComPtr<ID3D12Debug> m_debugController;
    
    std::unique_ptr<D3D12DescriptorManager> m_descriptorManager;
    std::vector<std::unique_ptr<D3D12CommandQueue>> m_commandQueues;
    
    bool m_useWarpDevice;
    bool m_enableDebugLayer;

public:
    D3D12Device();
    ~D3D12Device() override;
    
    bool Initialize(const DeviceDesc& desc) override;
    void Shutdown() override;
    
    // 资源创建
    ICommandQueue* CreateCommandQueue(COMMAND_QUEUE_TYPE type) override;
    ISwapChain* CreateSwapChain(const SwapChainDesc& desc) override;
    IBuffer* CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr) override;
    ITexture* CreateTexture(const TextureDesc& desc, const void* initialData = nullptr) override;
    IShader* CreateShader(const ShaderDesc& desc) override;
    IPipelineState* CreatePipelineState(const PipelineStateDesc& desc) override;
    IRootSignature* CreateRootSignature(const RootSignatureDesc& desc) override;
    IDescriptorHeap* CreateDescriptorHeap(const DescriptorHeapDesc& desc) override;
    IFence* CreateFence(uint64_t initialValue = 0) override;
    
    // 查询信息
    API_TYPE GetAPI() const override { return API_TYPE::D3D12; }
    uint32_t GetDescriptorHandleIncrementSize(DESCRIPTOR_HEAP_TYPE type) const override;
    
    // 原生访问
    ID3D12Device* GetNativeDevice() { return m_device.Get(); }
    IDXGIFactory4* GetDXGIFactory() { return m_dxgiFactory.Get(); }
    
private:
    bool CreateDXGIFactory();
    bool CreateDevice(bool useWarp);
    bool EnableDebugLayer();
    void ReportLiveObjects();
};

bool D3D12Device::Initialize(const DeviceDesc& desc) {
    m_useWarpDevice = desc.useWarpDevice;
    m_enableDebugLayer = desc.enableDebugLayer;
    
    UINT dxgiFactoryFlags = 0;
    
    if (m_enableDebugLayer) {
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController)))) {
            m_debugController->EnableDebugLayer();
            dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
    }
    
    // 创建DXGI工厂
    if (FAILED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_dxgiFactory)))) {
        return false;
    }
    
    // 创建设备
    if (m_useWarpDevice) {
        ComPtr<IDXGIAdapter> warpAdapter;
        ThrowIfFailed(m_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
        ThrowIfFailed(D3D12CreateDevice(
            warpAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device)
        ));
    } else {
        ComPtr<IDXGIAdapter1> hardwareAdapter;
        GetHardwareAdapter(m_dxgiFactory.Get(), &hardwareAdapter);
        
        if (FAILED(D3D12CreateDevice(
            hardwareAdapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&m_device)
        ))) {
            return false;
        }
    }
    
    // 检查功能支持
    D3D12_FEATURE_DATA_D3D12_OPTIONS options = {};
    if (SUCCEEDED(m_device->CheckFeatureSupport(
        D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) {
        // 存储支持的功能
    }
    
    // 创建描述符管理器
    m_descriptorManager = std::make_unique<D3D12DescriptorManager>(m_device.Get());
    
    return true;
}

void D3D12Device::Shutdown() {
    // 等待GPU完成所有工作
    for (auto& queue : m_commandQueues) {
        queue->WaitIdle();
    }
    
    // 报告泄漏的对象（调试模式）
    if (m_enableDebugLayer) {
        ReportLiveObjects();
    }
    
    m_descriptorManager.reset();
    m_commandQueues.clear();
    m_dxgiFactory.Reset();
    m_device.Reset();
    
    if (m_debugController) {
        m_debugController->Release();
        m_debugController = nullptr;
    }
}