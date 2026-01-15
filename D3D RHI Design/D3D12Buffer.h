class D3D12Buffer : public IBuffer {
private:
    ComPtr<ID3D12Resource> m_resource;
    D3D12_RESOURCE_STATES m_currentState;
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

public:
    D3D12Buffer(ID3D12Device* device, const BufferDesc& desc);
    
    ID3D12Resource* GetNativeResource() const { return m_resource.Get(); }
    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const { return m_vertexBufferView; }
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const { return m_indexBufferView; }
    
    void* Map();
    void Unmap();
};

class D3D12Texture : public ITexture {
private:
    ComPtr<ID3D12Resource> m_resource;
    D3D12_RESOURCE_STATES m_currentState;
    DXGI_FORMAT m_format;

public:
    D3D12Texture(ID3D12Device* device, const TextureDesc& desc);
    
    ID3D12Resource* GetNativeResource() const { return m_resource.Get(); }
    DXGI_FORMAT GetFormat() const { return m_format; }
    
    D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandle() const;
    D3D12_CPU_DESCRIPTOR_HANDLE GetUAVHandle() const;
};