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
import :Log;
import :Utils;
import :EngineDefinition;


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

            m_pFence = MakeSharedPtr<D3D12Fence>(m_pEngine);

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
    RHIRootSignaturePtr D3D12Context::CreateRootSignarue(RHIRootSignatureDesc desc)
    {
        return MakeSharedPtr<D3D12RootSignature>(m_pEngine, desc);
    }
    RHIPipelineStatePtr D3D12Context::CreateGraphicPipelineState(RHIGraphicsPipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL( ((D3D12PipelineState*)pipeline.get())->CreateGraphicsPipelineState());
        }
        return pipeline;
    }
    RHIPipelineStatePtr D3D12Context::CreateComputePipelineState(RHIComputePipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL(((D3D12PipelineState*)pipeline.get())->CreateComputePipelineState());
        }
        return pipeline;
    }
    RHIPipelineStatePtr D3D12Context::CreateRayTracingPipelineState(RHIRayTracingPipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL(((D3D12PipelineState*)pipeline.get())->CreateRayTracingPipelineState());
        }
        return pipeline;
    }
    RHIPipelineStatePtr D3D12Context::CreateMeshShaderPipelineState(RHIMeshShaderPipelineDesc desc)
    {
        RHIPipelineStatePtr pipeline = MakeSharedPtr<D3D12PipelineState>(m_pEngine, desc);
        if (pipeline)
        {
            AETHER_RET_NULL_IF_FAIL(((D3D12PipelineState*)pipeline.get())->CreateMeshPipelineState());
        }
        return pipeline;
    }
    RHICommandListPtr D3D12Context::CreateGraphicCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Graphics);
    }
    RHICommandListPtr D3D12Context::CreateComputeCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Compute);
    }
    RHICommandListPtr D3D12Context::CreateCopyCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Copy);
    }
    RHICommandListPtr D3D12Context::CreateBundleCommandList()
    {
        return MakeSharedPtr<D3D12CommandList>(m_pEngine, ECommandListType::Bundle);
    }






    RHIShaderPtr D3D12Context::CreateShader(EShaderStage stage, std::string const& name, std::string const& entry_func_name, const void* byteCode, size_t byteCodeSize)
    {
        return MakeSharedPtr<D3D12Shader>(m_pEngine, stage, name, entry_func_name, byteCode, byteCodeSize);
    }
    RHIMeshPtr D3D12Context::CreateMesh()
    {
        return MakeSharedPtr<D3D12Mesh>(m_pEngine);
    }

    RHIBufferPtr D3D12Context::CreateConstantBuffer(ResourceFlags flags, uint32_t data_size, const void* data)
    {
        return nullptr;
    }
    RHIBufferPtr D3D12Context::CreateVertexBuffer(uint32_t data_size, const void* data)
    {
        RHIBufferPtr buf = MakeSharedPtr<D3D12VertexBuffer>(m_pEngine, data_size);
        buf->Create(data_size, data);
        return buf;
    }
    RHIBufferPtr D3D12Context::CreateIndexBuffer(uint32_t data_size, const  void* data)
    {
        RHIBufferPtr buf = MakeSharedPtr<D3D12IndexBuffer>(m_pEngine, data_size);
        buf->Create(data_size, data);
        return buf;
    }











    AResult D3D12Context::CheckCapabilitySetSupport()
    {
        return A_Success;
    }

};