class IResource {
public:
    virtual ~IResource() = default;
    
    virtual RESOURCE_STATE GetCurrentState() const = 0;
    virtual void SetCurrentState(RESOURCE_STATE state) = 0;
    virtual size_t GetSize() const = 0;
    virtual FORMAT GetFormat() const = 0;
    virtual const ResourceDesc& GetDesc() const = 0;
    
    virtual void* Map(uint32_t subresource = 0) = 0;
    virtual void Unmap(uint32_t subresource = 0) = 0;
};

class IBuffer : public IResource {
public:
    virtual BUFFER_TYPE GetType() const = 0;
    virtual uint32_t GetStride() const = 0;
    virtual uint32_t GetElementCount() const = 0;
};

class ITexture : public IResource {
public:
    virtual TEXTURE_TYPE GetType() const = 0;
    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual uint32_t GetDepth() const = 0;
    virtual uint32_t GetMipLevels() const = 0;
    virtual uint32_t GetArraySize() const = 0;
    virtual uint32_t GetSampleCount() const = 0;
    virtual uint32_t GetSampleQuality() const = 0;
};