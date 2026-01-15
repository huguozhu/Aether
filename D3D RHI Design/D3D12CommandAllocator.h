class D3D12CommandAllocator : public ICommandAllocator {
private:
    ComPtr<ID3D12CommandAllocator> m_allocator;
    D3D12_COMMAND_LIST_TYPE m_type;
    std::atomic<bool> m_isInUse;

public:
    D3D12CommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
    ~D3D12CommandAllocator();

    void Reset() override;
    
    // D3D12特定方法
    ID3D12CommandAllocator* GetNativeAllocator() const { return m_allocator.Get(); }
    bool IsInUse() const { return m_isInUse; }
    void SetInUse(bool inUse) { m_isInUse = inUse; }
};