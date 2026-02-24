module;
#include <windows.h>

module Aether:D3D12Context;
import :D3D12Context;
import :D3D12Definition;
import :D3D12Window;
import :D3D12Shader;
import :D3D12Mesh;
import :D3D12Resource;
import :D3D12CommandList;
import :D3D12RootSignature;
import :D3D12PipelineState;
import :D3D12Buffer;
import :D3D12Texture;
import :Log;
import :Utils;
import :EngineDefinition;
import :RHIFence;
import std;


const char* GetD3D12FeatureLevelStr(D3D_FEATURE_LEVEL feature_level)
{
    switch (feature_level)
    {
    case D3D_FEATURE_LEVEL_12_2:
        return "12.2";
    case D3D_FEATURE_LEVEL_12_1:
        return "12.1";
    case D3D_FEATURE_LEVEL_12_0:
        return "12.0";
    default:
        return "unknown";
    }
}

#include "Utils/Macros.h"
namespace Aether
{

    D3D12Context::D3D12Context(AetherEngine* engine)
        :RHIContext(engine)
    {
        m_pRtvDescAllocator = MakeSharedPtr<D3D12GpuDescriptorAllocator>(m_pEngine, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
        m_pDsvDescAllocator = MakeSharedPtr<D3D12GpuDescriptorAllocator>(m_pEngine, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
        m_pCbvSrvUavDescAllocator = MakeSharedPtr<D3D12GpuDescriptorAllocator>(m_pEngine, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
        m_pDynamicCbvSrvUavDescAllocator = MakeSharedPtr<D3D12GpuDescriptorAllocator>(m_pEngine, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
        m_pSamplerDescAllocator = MakeSharedPtr<D3D12GpuDescriptorAllocator>(m_pEngine, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);

        m_pUploadMemoryAllocator = MakeSharedPtr<D3D12GpuMemoryAllocator>(m_pEngine, true);
        m_pReadbackMemoryAllocator = MakeSharedPtr<D3D12GpuMemoryAllocator>(m_pEngine, false);
    }
    
    // Render Thread 
    ID3D12CommandAllocator* D3D12Context::D3DRenderCmdAllocator() const
    {
        return this->CurThreadContext(true).D3DCmdAllocator(m_iCurFrameIndex);
    }
    ID3D12GraphicsCommandList* D3D12Context::D3DRenderCmdList() const
    {
        return this->CurThreadContext(true).D3DCmdList();
    }
    void D3D12Context::CommitRenderCmd()
    {
        this->CommitCmd(this->CurThreadContext(true));
    }
    void D3D12Context::SyncRenderCmd()
    {
        this->SyncCmd(this->CurThreadContext(true));
    }
    void D3D12Context::ResetRenderCmd()
    {
        this->ResetCmd(this->CurThreadContext(true));
    }

    // Load Thread 
    ID3D12CommandAllocator* D3D12Context::D3DLoadCmdAllocator() const
    {
        return this->CurThreadContext(false).D3DCmdAllocator(m_iCurFrameIndex);
    }
    ID3D12GraphicsCommandList* D3D12Context::D3DLoadCmdList() const
    {
        return this->CurThreadContext(false).D3DCmdList();
    }
    void D3D12Context::CommitLoadCmd()
    {
        this->CommitCmd(this->CurThreadContext(false));
    }
    void D3D12Context::SyncLoadCmd()
    {
        this->SyncCmd(this->CurThreadContext(false));
    }
    void D3D12Context::ResetLoadCmd()
    {
        this->ResetCmd(this->CurThreadContext(false));
    }











    D3D12GpuDescriptorBlock D3D12Context::AllocRtvDescBlock(uint32_t size)
    {
        return m_pRtvDescAllocator->Allocate(size);
    }
    void D3D12Context::DeallocRtvDescBlock(D3D12GpuDescriptorBlock&& desc_block)
    {
        //m_pRtvDescAllocator->Deallocate(std::move(desc_block), m_iFrameFenceValue);
    }
    void D3D12Context::RenewRtvDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size)
    {
        //m_pRtvDescAllocator->Renew(desc_block, m_iFrameFenceValue, size);
    }
    D3D12GpuDescriptorBlock D3D12Context::AllocDsvDescBlock(uint32_t size)
    {
        return m_pDsvDescAllocator->Allocate(size);
    }
    void D3D12Context::DeallocDsvDescBlock(D3D12GpuDescriptorBlock&& desc_block)
    {
        //m_pDsvDescAllocator->Deallocate(std::move(desc_block), m_iFrameFenceValue);
    }
    void D3D12Context::RenewDsvDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size)
    {
        //m_pDsvDescAllocator->Renew(desc_block, m_iFrameFenceValue, size);
    }
    D3D12GpuDescriptorBlock D3D12Context::AllocCbvSrvUavDescBlock(uint32_t size)
    {
        return m_pCbvSrvUavDescAllocator->Allocate(size);
    }
    void D3D12Context::DeallocCbvSrvUavDescBlock(D3D12GpuDescriptorBlock&& desc_block)
    {
        //m_pCbvSrvUavDescAllocator->Deallocate(std::move(desc_block), m_iFrameFenceValue);
    }
    void D3D12Context::RenewCbvSrvUavDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size)
    {
        //m_pCbvSrvUavDescAllocator->Renew(desc_block, m_iFrameFenceValue, size);
    }
    D3D12GpuDescriptorBlock D3D12Context::AllocDynamicCbvSrvUavDescBlock(uint32_t size)
    {
        return m_pDynamicCbvSrvUavDescAllocator->Allocate(size);
    }
    void D3D12Context::DeallocDynamicCbvSrvUavDescBlock(D3D12GpuDescriptorBlock&& desc_block)
    {
        //m_pDynamicCbvSrvUavDescAllocator->Deallocate(std::move(desc_block), m_iFrameFenceValue);
    }
    void D3D12Context::RenewDynamicCbvSrvUavDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size)
    {
        //m_pDynamicCbvSrvUavDescAllocator->Renew(desc_block, m_iFrameFenceValue, size);
    }

    D3D12GpuDescriptorBlock D3D12Context::AllocSamplerDescBlock(uint32_t size)
    {
        return m_pSamplerDescAllocator->Allocate(size);
    }
    void D3D12Context::DeallocSamplerDescBlock(D3D12GpuDescriptorBlock&& desc_block)
    {
        //m_pSamplerDescAllocator->Deallocate(std::move(desc_block), m_iFrameFenceValue);
    }
    void D3D12Context::RenewSamplerDescBlock(D3D12GpuDescriptorBlock& desc_block, uint32_t size)
    {
        //m_pSamplerDescAllocator->Renew(desc_block, m_iFrameFenceValue, size);
    }


    D3D12GpuMemoryBlock D3D12Context::AllocUploadMemBlock(uint32_t size_in_bytes, uint32_t alignment)
    {
        return m_pUploadMemoryAllocator->Allocate(size_in_bytes, alignment);
    }
    void D3D12Context::DeallocUploadMemBlock(D3D12GpuMemoryBlock&& mem_block)
    {
    }
    void D3D12Context::RenewUploadMemBlock(D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment)
    {
    }
    D3D12GpuMemoryBlock D3D12Context::AllocReadbackMemBlock(uint32_t size_in_bytes, uint32_t alignment)
    {
        return m_pReadbackMemoryAllocator->Allocate(size_in_bytes, alignment);
    }
    void D3D12Context::DeallocReadbackMemBlock(D3D12GpuMemoryBlock&& mem_block)
    {

    }
    void D3D12Context::RenewReadbackMemBlock(D3D12GpuMemoryBlock& mem_block, uint32_t size_in_bytes, uint32_t alignment)
    {
    }

    std::vector<D3D12_RESOURCE_BARRIER>* D3D12Context::FindResourceBarriers(ID3D12GraphicsCommandList* cmd_list, bool allow_creation)
    {
        auto iter = m_vResBarriers.begin();
        for (; iter != m_vResBarriers.end(); ++iter)
        {
            if (iter->first == cmd_list)
                break;
        }

        std::vector<D3D12_RESOURCE_BARRIER>* ret;
        if (iter == m_vResBarriers.end())
        {
            if (allow_creation)
                ret = &m_vResBarriers.emplace_back(cmd_list, std::vector<D3D12_RESOURCE_BARRIER>()).second;
            else
                ret = nullptr;
        }
        else
            ret = &iter->second;
        return ret;
    }
    void D3D12Context::FlushResourceBarriers(ID3D12GraphicsCommandList* cmd_list)
    {
        std::vector<D3D12_RESOURCE_BARRIER>* res_barriers = this->FindResourceBarriers(cmd_list, false);
        if (res_barriers && !res_barriers->empty())
        {
            cmd_list->ResourceBarrier(static_cast<UINT>(res_barriers->size()), res_barriers->data());
            res_barriers->clear();
        }
    }
    void D3D12Context::AddResourceBarrier(ID3D12GraphicsCommandList* cmd_list, std::span<D3D12_RESOURCE_BARRIER> barriers)
    {
        std::vector<D3D12_RESOURCE_BARRIER>* res_barriers = this->FindResourceBarriers(cmd_list, true);
        res_barriers->insert(res_barriers->end(), barriers.begin(), barriers.end());
    }
    void D3D12Context::AddStallResource(ID3D12ResourcePtr const& resource)
    {
        if (resource)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_vStallResources.push_back(resource);
        }
    }


    AResult D3D12Context::Init()
    {
        AETHER_RETIF_FAIL(DxgiHelper::Init(m_pEngine->GetPreferredAdapter(), m_pEngine->EnableDebug()));
        do {
            D3D_FEATURE_LEVEL feature_levels[] =
            {
                D3D_FEATURE_LEVEL_12_2,
                D3D_FEATURE_LEVEL_12_1,
                D3D_FEATURE_LEVEL_12_0,
            };
            uint32_t feature_level_start_index = 0;
            HRESULT hr = S_OK;
            uint32_t feature_level_count = sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL);
            for (; feature_level_start_index < feature_level_count; feature_level_start_index++)
            {
                hr = D3D12CreateDevice(m_vAdapterList[m_iCurAdapterNo]->DXGIAdapter(),
                    feature_levels[feature_level_start_index], __uuidof(ID3D12Device),
                    (void**)m_pDevice.GetAddressOf());
                if (SUCCEEDED(hr))
                {
                    hr = m_pDevice->QueryInterface(IID_PPV_ARGS(&m_pDevice5));
                    LOG_WARNING("Couldn't get DirectX Raytracing interface for the device.");
                    break;
                }
            }
            if (FAILED(hr))
            {
                LOG_ERROR("D3D12CreateDevice Error, hr:%x", hr);
                break;
            }
            LOG_INFO("device supported feature level %s", GetD3D12FeatureLevelStr(feature_levels[feature_level_start_index]));

            D3D12_COMMAND_QUEUE_DESC queue_desc = {};
            queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
            hr = m_pDevice->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_pCommandQueue));
            if (FAILED(hr))
            {
                LOG_ERROR("CreateCommandQueue Error, hr:%x", hr);
                break;
            }

            m_pFrameFence = MakeSharedPtr<D3D12Fence>(m_pEngine);

            // Create descriptor heaps.
            {
                // Describe and create a render target view (RTV) descriptor heap.
                D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
                rtv_heap_desc.NumDescriptors = RHIContext::NUM_BACK_BUFFERS;
                rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
                rtv_heap_desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
                m_pDevice->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&m_pRtvHeap));
                m_iRtvDescSize = m_pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
            }

            this->CheckCapabilitySetSupport();
        } while (0);

        return A_Success;
    }
    AResult D3D12Context::AttachNativeWindows(std::string const& name, void* native_wnd)
    {
        AResult res = A_Success;
        D3DAdapterPtr pAdapter = this->ActiveAdapter();
        if (native_wnd)
        {
            m_pPhysicalWindow = MakeSharedPtr<D3D12Window>(m_pEngine);
            res = m_pPhysicalWindow->Create(pAdapter.get(), name, native_wnd);
            if (AETHER_CHECKFAILED(res))
                return res;
        }
        m_pScreenFB = m_pPhysicalWindow;
        return res;
    }
    AResult D3D12Context::SwapBuffers()
    {
        if (m_pPhysicalWindow)
        {
            AETHER_RETIF_FAIL(m_pPhysicalWindow->SwapBuffers());
        }
        return A_Success;
    }
    AResult D3D12Context::BeginRenderPass(const RenderPassInfo& renderPassInfo)
    {
        if (!renderPassInfo.fb)
            return ERR_INVALID_DATA;

        D3D12FrameBuffer* pFB = static_cast<D3D12FrameBuffer*>(renderPassInfo.fb);
        ID3D12GraphicsCommandList* cmd_list = this->D3DRenderCmdList();

        pFB->Active(cmd_list);
        return A_Success;
    }
    AResult D3D12Context::EndRenderPass()
    {
        return A_Success;
    }


    AResult D3D12Context::CheckCapabilitySetSupport()
    {
        return A_Success;
    }





    /******************************************************************************
    * D3D12Context::PerThreadContext
    *******************************************************************************/
    D3D12Context::PerThreadContext::PerThreadContext(ID3D12Device* d3d_device, RHIFencePtr const& frame_fence)
        : m_ThreadId(std::this_thread::get_id()), m_pFrameFence(frame_fence)
    {
        for (auto& context : m_vPerFrameContexts)
        {
            ThrowIfFailed(d3d_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
                IID_PPV_ARGS(context.d3d_cmd_allocator.ReleaseAndGetAddressOf())));
        }
        ThrowIfFailed(d3d_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_vPerFrameContexts[0].d3d_cmd_allocator.Get(),
            nullptr, IID_PPV_ARGS(m_pD3dCmdList.ReleaseAndGetAddressOf())));
    }
    D3D12Context::PerThreadContext::~PerThreadContext()
    {
        if (auto fence = m_pFrameFence.lock())
        {
            uint64_t max_fence_val = 0;
            for (auto const& context : m_vPerFrameContexts)
            {
                max_fence_val = std::max(max_fence_val, context.fence_value);
            }
            fence->Wait(max_fence_val);
            m_pFrameFence.reset();
        }

        m_pD3dCmdList.Reset();
        for (auto& context : m_vPerFrameContexts)
        {
            context.d3d_cmd_allocator.Reset();
            context.fence_value = 0;
        }
    }
    void D3D12Context::PerThreadContext::CommitCmd(ID3D12CommandQueue* d3d_cmd_queue, uint32_t frame_index)
    {
        ThrowIfFailed(m_pD3dCmdList->Close());
        ID3D12CommandList* cmd_lists[] = { m_pD3dCmdList.Get() };
        d3d_cmd_queue->ExecuteCommandLists(static_cast<uint32_t>(std::size(cmd_lists)), cmd_lists);
        m_vPerFrameContexts[frame_index].fence_value = static_cast<D3D12Fence&>(*m_pFrameFence.lock()).Signal(d3d_cmd_queue);
    }
    void D3D12Context::PerThreadContext::SyncCmd(uint32_t frame_index)
    {
        m_pFrameFence.lock()->Wait(m_vPerFrameContexts[frame_index].fence_value);
    }

    void D3D12Context::PerThreadContext::ResetCmd(uint32_t frame_index)
    {
        m_pD3dCmdList->Reset(this->D3DCmdAllocator(frame_index), nullptr);
    }
    void D3D12Context::PerThreadContext::Reset(uint32_t frame_index)
    {
        this->SyncCmd(frame_index);
        this->D3DCmdAllocator(frame_index)->Reset();
    }
    ID3D12CommandAllocator* D3D12Context::PerThreadContext::D3DCmdAllocator(uint32_t frame_index) const
    {
        return m_vPerFrameContexts[frame_index].d3d_cmd_allocator.Get();
    }
    ID3D12GraphicsCommandList* D3D12Context::PerThreadContext::D3DCmdList() const
    {
        return m_pD3dCmdList.Get();
    }
    uint64_t D3D12Context::PerThreadContext::FrameFenceValue(uint32_t frame_index) const
    {
        return m_vPerFrameContexts[frame_index].fence_value;
    }

    D3D12Context::PerFrameContext::~PerFrameContext()
    {
        m_vStallResources.clear();
    }
    void D3D12Context::PerFrameContext::AddStallResource(ID3D12ResourcePtr const& resource)
    {
        if (resource)
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_vStallResources.push_back(resource);
        }
    }
    void D3D12Context::PerFrameContext::ClearStallResources()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_vStallResources.clear();
    }
    D3D12Context::PerThreadContext& D3D12Context::CurThreadContext(bool is_render_context) const
    {
        std::thread::id thread_id = std::this_thread::get_id();
        auto& thread_cmd_contexts = is_render_context ? m_vRenderThreadCmdContexts : m_vLoadThreadCmdContexts;
        auto iter = std::find_if(thread_cmd_contexts.begin(), thread_cmd_contexts.end(),
            [&thread_id](std::unique_ptr<PerThreadContext> const& context) { return context->ThreadID() == thread_id; });
        if (iter == thread_cmd_contexts.end())
        {
            auto new_context = MakeUniquePtr<PerThreadContext>(m_pDevice.Get(), m_pFrameFence);
            if (!is_render_context)
            {
                new_context->D3DCmdList()->Close();
            }

            thread_cmd_contexts.emplace_back(std::move(new_context));
            iter = thread_cmd_contexts.end() - 1;
        }
        return *(*iter);
    }
    void D3D12Context::CommitCmd(PerThreadContext& context)
    {
        context.CommitCmd(m_pCommandQueue.Get(), m_iCurFrameIndex);
        m_iFrameFenceValue = context.FrameFenceValue(m_iCurFrameIndex);
    }
    void D3D12Context::SyncCmd(PerThreadContext& context)
    {
        context.SyncCmd(m_iCurFrameIndex);
    }
    void D3D12Context::ResetCmd(PerThreadContext& context)
    {
        context.ResetCmd(m_iCurFrameIndex);
    }

};