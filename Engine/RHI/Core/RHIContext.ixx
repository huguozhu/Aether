export module Aether:RHIContext;

import :RHIStruct;
import :RHIDevice;
import :RHICommandList;
import :RHIResource;
import :Error;
import :Engine;
import <string>;
import <vector>;

export namespace Aether
{
    class AetherEngine;
    class RHIContext
    {
    public:
        RHIContext(AetherEngine* engine);
        virtual ~RHIContext() = default;

        virtual AResult Init() = 0;

        virtual AResult AttachNativeWindows(std::string const& name, void* native_wnd) = 0;


        CapabilitySet const& GetCapabilitySet() const { return m_CapabilitySet; }
        virtual AResult CheckCapabilitySetSupport() { return A_Success; }

        static constexpr uint32_t const NUM_BACK_BUFFERS = 2;
    protected:
        AetherEngine* m_pEngine = nullptr;
        
        CapabilitySet m_CapabilitySet;

        // 当前渲染状态缓存（用于减少重复设置）
        struct {
            void* pipelineState = nullptr;
            std::vector<RHIResource*> shaderResources[8];     // 各槽位资源
            std::vector<RHIBuffer*> constantBuffers[16];
            std::vector<void*> samplers[16];
            EResourceState renderTargetState = EResourceState::RenderTarget;
            EResourceState depthStencilState = EResourceState::DepthWrite;
        } currentState;

        // 资源状态追踪器（D3D12/Vulkan 需要显式管理）
        //RHIResourceStateTracker stateTracker;
    };

};