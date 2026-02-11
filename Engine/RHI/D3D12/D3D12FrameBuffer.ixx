export module Aether:D3D12FrameBuffer;
import :RHIFrameBuffer;
import :Engine;
import :EngineDefinition;
import :D3D12Definition;

export namespace Aether
{
    class D3D12Resource;
    class D3D12FrameBuffer : public RHIFrameBuffer
    {
    public:
        D3D12FrameBuffer(AetherEngine* engine);
        void BindBarrier(ID3D12GraphicsCommandList* cmd_list);
        AResult Active(D3D12CommandList* cmd_list);

    protected:
        void Update();
        
    protected:
        AetherEngine* m_pEngine = nullptr;
        
        /*Buffer\first_subres\num_subres*/
        std::vector<std::tuple<D3D12Resource*, uint32_t, uint32_t> > m_vD3dRtvResources;
        std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> m_vD3dRtvCpuHandles;

        std::tuple<D3D12Resource*, uint32_t, uint32_t> m_DsvResource;        
        D3D12_CPU_DESCRIPTOR_HANDLE m_D3dSdvHandle;
        D3D12_CPU_DESCRIPTOR_HANDLE* m_D3dSdvHandlePtr = nullptr;
        
        D3D12_VIEWPORT m_D3dViewport;

        // for Pipeline State Object
        uint32_t m_iNumRtvs;
        std::array<DXGI_FORMAT, MAX_COLOR_ATTACHMENTS> m_vRtvFormats;
        DXGI_FORMAT m_eDsvFormat;
        uint32_t m_iNumSamples;
    };
};
