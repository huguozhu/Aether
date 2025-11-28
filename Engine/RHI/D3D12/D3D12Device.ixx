export module Aether.RHI:D3D12Device;

import :RHIDevice;
import :D3D12Definition;

import <windows.h>;

export namespace Aether
{
    class D3D12Device : public RHIDevice
    {
    public:
        ID3D12DevicePtr                 m_pDevice;
        ID3D12CommandQueuePtr           m_pCommandQueue;
        ID3D12CommandAllocatorPtr       m_pCmdAlloc;
        ID3D12GraphicsCommandListPtr    m_pCmdList;

        D3D12Device() {
            // 初始化 D3D12 设备（省略适配器枚举）
            CreateDXGIFactory1(IID_PPV_ARGS(m_pFactory.GetAddressOf()));
            m_pFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
                IID_PPV_ARGS(m_pAdapter.GetAddressOf()));

            D3D12CreateDevice(m_pAdapter.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_pDevice));

            D3D12_COMMAND_QUEUE_DESC queueDesc = {};
            queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
            m_pDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

            m_pDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCmdAlloc));
            m_pDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCmdAlloc.Get(), nullptr,
                IID_PPV_ARGS(&m_pCmdList));
            m_pCmdList->Close();
        }

        // --- 着色器创建 ---
        void* CreateMeshShader(const RHIShaderBytecode& bc) override {
            ID3DBlobPtr blob;
            D3DCreateBlob(bc.size, blob.GetAddressOf());
            memcpy(blob->GetBufferPointer(), bc.code, bc.size);

            D3D12_SHADER_BYTECODE shaderBytecode;
            shaderBytecode.pShaderBytecode = blob->GetBufferPointer();
            shaderBytecode.BytecodeLength = blob->GetBufferSize();

            // 缓存 Blob 防止释放
            m_vMeshShaders.push_back(blob);
            return new D3D12_SHADER_BYTECODE(shaderBytecode);
        }

        // --- Mesh Pipeline 创建 ---
        void* CreateMeshPipeline(const RHIMeshPipelineDesc& desc) override {
            D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
            psoDesc.pRootSignature = m_pRootSignature.Get(); // 假设已创建
            psoDesc.NodeMask = 0;
            psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

            // 设置着色器
            //if (desc.vertexShader) psoDesc.VS = *static_cast<D3D12_SHADER_BYTECODE*>(desc.amplificationShader);
            //if (desc.meshShader)  psoDesc.MS = *static_cast<D3D12_SHADER_BYTECODE*>(desc.meshShader);
            //if (desc.pixelShader) psoDesc.PS = *static_cast<D3D12_SHADER_BYTECODE*>(desc.pixelShader);
            //if (desc.amplificationShader) psoDesc.AS = *static_cast<D3D12_SHADER_BYTECODE*>(desc.taskShader);

            //// 输入布局、RTV、DSV、Rasterizer、Blend 等略
            //psoDesc.InputLayout = ...;
            //psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
            //psoDesc.NumRenderTargets = 1;
            //psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
            //psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

            //ComPtr<ID3D12PipelineState> pso;
            //device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&pso));
            return nullptr;// pso.Detach();
        }

        // --- 光线追踪 Pipeline ---
        void* CreateRayTracingPipeline(const RHIRayTracingPipelineDesc& desc) override {
            std::vector<D3D12_SUBOBJECT_TO_EXPORTS_ASSOCIATION> soExports;
            std::vector<D3D12_STATE_SUBOBJECT> subobjects;

            // 每个 Shader Config
            D3D12_RAYTRACING_SHADER_CONFIG shaderConfig = {};
            shaderConfig.MaxPayloadSizeInBytes = 32;
            shaderConfig.MaxAttributeSizeInBytes = 32;

            D3D12_STATE_SUBOBJECT shaderConfigSo = {};
            shaderConfigSo.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_SHADER_CONFIG;
            shaderConfigSo.pDesc = &shaderConfig;
            subobjects.push_back(shaderConfigSo);

            // Pipeline Config
            D3D12_RAYTRACING_PIPELINE_CONFIG pipeConfig = { desc.maxRecursionDepth };
            D3D12_STATE_SUBOBJECT pipeConfigSo = {};
            pipeConfigSo.Type = D3D12_STATE_SUBOBJECT_TYPE_RAYTRACING_PIPELINE_CONFIG;
            pipeConfigSo.pDesc = &pipeConfig;
            subobjects.push_back(pipeConfigSo);

            // 全局根签名
            D3D12_STATE_SUBOBJECT globalRootSigSo = {};
            globalRootSigSo.Type = D3D12_STATE_SUBOBJECT_TYPE_GLOBAL_ROOT_SIGNATURE;
            globalRootSigSo.pDesc = m_pRootSignature.Get();
            subobjects.push_back(globalRootSigSo);

            // 最后：创建 PSO
            D3D12_STATE_OBJECT_DESC rtPipelineDesc = {};
            rtPipelineDesc.Type = D3D12_STATE_OBJECT_TYPE_RAYTRACING_PIPELINE;
            rtPipelineDesc.pSubobjects = subobjects.data();
            rtPipelineDesc.NumSubobjects = static_cast<UINT>(subobjects.size());

            ID3D12StateObjectPtr rtPSO;
            //m_pDevice->CreateStateObject(&rtPipelineDesc, IID_PPV_ARGS(&rtPSO));
            return rtPSO.Detach();
        }

    private:
        IDXGIFactory6Ptr m_pFactory;
        IDXGIAdapter4Ptr m_pAdapter;
        ID3D12RootSignaturePtr m_pRootSignature;
        std::vector<ID3DBlobPtr> m_vMeshShaders;
    };

}