export module Aether.RHI:RHIDevice;

import :RHIStruct;
import <vector>;


export namespace Aether
{
    struct RHIShaderBytecode {
        const void* code;
        size_t size;
    };

    class RHIDevice {
    public:
        virtual ~RHIDevice() = default;

        // 创建着色器
        virtual void* CreateVertexShader(const RHIShaderBytecode& bc) = 0;
        virtual void* CreatePixelShader(const RHIShaderBytecode& bc) = 0;
        virtual void* CreateMeshShader(const RHIShaderBytecode& bc) = 0;
        virtual void* CreateRayGenShader(const RHIShaderBytecode& bc) = 0;

        // 创建管线
        virtual void* CreateGraphicsPipeline(const struct RHIGraphicsPipelineDesc& desc) = 0;
        virtual void* CreateComputePipeline(const struct RHIComputePipelineDesc& desc) = 0;
        virtual void* CreateMeshPipeline(const struct RHIMeshPipelineDesc& desc) = 0;
        virtual void* CreateRayTracingPipeline(const struct RHIRayTracingPipelineDesc& desc) = 0;

        // 资源创建
        virtual void* CreateBuffer(size_t size, EResourceState state, bool bUAV = false) = 0;
        virtual void* CreateTexture(int w, int h, int format, EResourceState state) = 0;
        virtual void* CreateSampler(const struct RHISamplerDesc& desc) = 0;

        // 命令列表
        virtual class RHICommandList* CreateCommandList() = 0;
        virtual void SubmitCommandLists(std::vector<RHICommandList*>& lists) = 0;

        virtual void WaitIdle() = 0;
    };

};