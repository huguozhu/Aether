class D3D12Renderer {
private:
    std::unique_ptr<D3D12Device> m_device;
    std::unique_ptr<D3D12SwapChain> m_swapChain;
    
    // 资源
    std::unique_ptr<D3D12Texture> m_renderTarget;
    std::unique_ptr<D3D12Texture> m_depthStencil;
    std::unique_ptr<D3D12Buffer> m_vertexBuffer;
    std::unique_ptr<D3D12Buffer> m_indexBuffer;
    std::unique_ptr<D3D12Buffer> m_constantBuffer;
    
    // 着色器和管线状态
    std::unique_ptr<D3D12Shader> m_vertexShader;
    std::unique_ptr<D3D12Shader> m_pixelShader;
    std::unique_ptr<D3D12RootSignature> m_rootSignature;
    std::unique_ptr<D3D12PipelineState> m_pipelineState;
    
    // 描述符
    std::unique_ptr<D3D12DescriptorHeap> m_rtvHeap;
    std::unique_ptr<D3D12DescriptorHeap> m_dsvHeap;
    std::unique_ptr<D3D12DescriptorHeap> m_cbvSrvUavHeap;
    
    // 命令相关
    std::unique_ptr<D3D12CommandQueue> m_graphicsQueue;
    std::unique_ptr<D3D12CommandAllocator> m_commandAllocator;
    std::unique_ptr<D3D12CommandList> m_commandList;
    
    // 同步
    std::unique_ptr<D3D12Fence> m_fence;
    uint64_t m_fenceValue = 0;
    HANDLE m_fenceEvent = nullptr;

public:
    bool Initialize(HWND hwnd, uint32_t width, uint32_t height);
    void Shutdown();
    void Render();
    
private:
    bool CreateResources();
    bool CreateShaders();
    bool CreateRootSignature();
    bool CreatePipelineState();
    void CreateDescriptorHeaps();
    void PopulateCommandList();
    void WaitForPreviousFrame();
};

bool D3D12Renderer::Initialize(HWND hwnd, uint32_t width, uint32_t height) {
    // 创建设备
    DeviceDesc deviceDesc = {};
    deviceDesc.enableDebugLayer = true;
    
    m_device = std::make_unique<D3D12Device>();
    if (!m_device->Initialize(deviceDesc)) {
        return false;
    }
    
    // 创建交换链
    SwapChainDesc swapChainDesc = {};
    swapChainDesc.width = width;
    swapChainDesc.height = height;
    swapChainDesc.bufferCount = 2;
    swapChainDesc.format = FORMAT::R8G8B8A8_UNORM;
    swapChainDesc.windowHandle = hwnd;
    
    m_swapChain = std::make_unique<D3D12SwapChain>(m_device.get(), swapChainDesc);
    
    // 创建命令队列
    m_graphicsQueue = std::make_unique<D3D12CommandQueue>(
        m_device.get(), COMMAND_QUEUE_TYPE_GRAPHICS);
    
    // 创建命令分配器和列表
    m_commandAllocator = std::make_unique<D3D12CommandAllocator>(
        m_device.get(), COMMAND_LIST_TYPE_GRAPHICS);
    
    m_commandList = std::make_unique<D3D12CommandList>(
        m_device.get(), m_commandAllocator.get(), COMMAND_LIST_TYPE_GRAPHICS);
    
    // 创建围栏
    m_fence = std::make_unique<D3D12Fence>(m_device.get(), 0);
    m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
    
    // 创建描述符堆
    CreateDescriptorHeaps();
    
    // 创建资源
    if (!CreateResources()) {
        return false;
    }
    
    // 创建着色器
    if (!CreateShaders()) {
        return false;
    }
    
    // 创建根签名
    if (!CreateRootSignature()) {
        return false;
    }
    
    // 创建管线状态
    if (!CreatePipelineState()) {
        return false;
    }
    
    return true;
}

void D3D12Renderer::Render() {
    // 等待上一帧完成
    WaitForPreviousFrame();
    
    // 重置命令分配器
    m_commandAllocator->Reset();
    
    // 重置命令列表
    m_commandList->Reset(m_commandAllocator.get(), m_pipelineState.get());
    
    // 记录命令
    PopulateCommandList();
    
    // 执行命令列表
    ID3D12CommandList* commandLists[] = { m_commandList->GetNativeCommandList() };
    m_graphicsQueue->ExecuteCommandLists(1, commandLists);
    
    // 呈现
    m_swapChain->Present(1, 0);
    
    // 信号围栏
    m_fenceValue++;
    m_graphicsQueue->Signal(m_fence.get(), m_fenceValue);
}

void D3D12Renderer::PopulateCommandList() {
    // 设置资源屏障：渲染目标准备渲染
    ResourceBarrier barrier = {};
    barrier.type = ResourceBarrier::Type::TRANSITION;
    barrier.resource = m_renderTarget.get();
    barrier.transition.stateBefore = RESOURCE_STATE_PRESENT;
    barrier.transition.stateAfter = RESOURCE_STATE_RENDER_TARGET;
    
    m_commandList->ResourceBarrier(1, &barrier);
    
    // 设置渲染目标
    m_commandList->SetRenderTargets(1, &m_renderTarget, m_depthStencil.get());
    
    // 清除渲染目标
    float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
    m_commandList->ClearRenderTarget(m_renderTarget.get(), clearColor);
    m_commandList->ClearDepthStencil(m_depthStencil.get(), 1.0f, 0);
    
    // 设置视口和裁剪矩形
    Viewport viewport = { 0.0f, 0.0f, 800.0f, 600.0f, 0.0f, 1.0f };
    Rect scissorRect = { 0, 0, 800, 600 };
    m_commandList->SetViewport(viewport);
    m_commandList->SetScissorRect(scissorRect);
    
    // 设置管线状态
    m_commandList->SetPipelineState(m_pipelineState.get());
    
    // 设置图元拓扑
    m_commandList->SetPrimitiveTopology(PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    
    // 设置顶点和索引缓冲区
    m_commandList->SetVertexBuffers(0, 1, &m_vertexBuffer);
    m_commandList->SetIndexBuffer(m_indexBuffer);
    
    // 设置根签名和描述符
    m_commandList->SetRootSignature(m_rootSignature.get());
    
    // 绘制
    m_commandList->DrawIndexed(3, 1, 0, 0, 0);
    
    // 资源屏障：渲染目标准备呈现
    barrier.transition.stateBefore = RESOURCE_STATE_RENDER_TARGET;
    barrier.transition.stateAfter = RESOURCE_STATE_PRESENT;
    m_commandList->ResourceBarrier(1, &barrier);
    
    // 结束命令列表记录
    m_commandList->End();
}