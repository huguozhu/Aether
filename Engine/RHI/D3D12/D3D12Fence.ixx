export module Aether:D3D12Fence;
import :RHIFence;
import :D3D12Definition;
import std;

namespace Aether
{
    class D3D12Fence : public RHIFence
    {
    public:
        D3D12Fence(AetherEngine* engine);

        uint64_t Signal() override;
        void Wait(uint64_t value)override;
        bool IsCompleted(uint64_t value)override;

        uint64_t Signal(ID3D12CommandQueue* cmd_queue);

    protected:

        ID3D12FencePtr          m_pFence = nullptr;
        std::atomic<uint64_t>   m_iFenceValue;
        uint64_t                m_iLastCompletedValue = 0;
        HANDLE                  m_hFenceEvent = nullptr;
    };

};
