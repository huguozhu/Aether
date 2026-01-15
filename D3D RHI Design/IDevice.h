class IDevice {
public:
    virtual ~IDevice() = default;
    
    virtual bool Initialize(const DeviceDesc& desc) = 0;
    virtual void Shutdown() = 0;
    
    virtual ICommandQueue* CreateCommandQueue(COMMAND_QUEUE_TYPE type) = 0;
    virtual ISwapChain* CreateSwapChain(const SwapChainDesc& desc) = 0;
    virtual IBuffer* CreateBuffer(const BufferDesc& desc, const void* initialData = nullptr) = 0;
    virtual ITexture* CreateTexture(const TextureDesc& desc, const void* initialData = nullptr) = 0;
    virtual IShader* CreateShader(const ShaderDesc& desc) = 0;
    virtual IPipelineState* CreatePipelineState(const PipelineStateDesc& desc) = 0;
    virtual IRootSignature* CreateRootSignature(const RootSignatureDesc& desc) = 0;
    virtual IDescriptorHeap* CreateDescriptorHeap(const DescriptorHeapDesc& desc) = 0;
    virtual IFence* CreateFence(uint64_t initialValue = 0) = 0;
    
    virtual API_TYPE GetAPI() const = 0;
    virtual uint32_t GetDescriptorHandleIncrementSize(DESCRIPTOR_HEAP_TYPE type) const = 0;
    
    virtual void* GetNativeDevice() = 0;
};