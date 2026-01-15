class IRootSignature {
public:
    virtual ~IRootSignature() = default;
    
    virtual uint32_t GetParameterCount() const = 0;
    virtual const RootSignatureDesc& GetDesc() const = 0;
    
    virtual void* GetNativeRootSignature() = 0;
};

class IDescriptorHeap {
public:
    virtual ~IDescriptorHeap() = default;
    
    virtual DESCRIPTOR_HEAP_TYPE GetType() const = 0;
    virtual uint32_t GetCapacity() const = 0;
    virtual uint32_t GetSize() const = 0;
    
    virtual DescriptorHandle Allocate(uint32_t count = 1) = 0;
    virtual void Free(const DescriptorHandle& handle, uint32_t count = 1) = 0;
    
    virtual void* GetCPUHandle(uint32_t index) const = 0;
    virtual void* GetGPUHandle(uint32_t index) const = 0;
    virtual void* GetNativeHeap() = 0;
};

struct DescriptorHandle {
    uint32_t index = UINT32_MAX;
    void* cpuHandle = nullptr;
    void* gpuHandle = nullptr;  // 仅适用于ShaderVisible堆
    
    bool IsValid() const { return cpuHandle != nullptr; }
    bool IsShaderVisible() const { return gpuHandle != nullptr; }
};