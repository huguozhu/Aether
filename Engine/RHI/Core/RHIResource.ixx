export module Aether:RHIResource;
import :Engine;
import :EngineDefinition;
import std;

export namespace Aether
{
    using ResourceFlags = uint64_t;
    constexpr uint64_t RESOURCE_FLAG_NONE = uint64_t(0);

    constexpr uint64_t RESOURCE_FLAG_CPU_READ = uint64_t(0x0000000000000001);        // Copy data from gpu-buffer --> cpu-buffer
    constexpr uint64_t RESOURCE_FLAG_CPU_WRITE = uint64_t(0x0000000000000002);        // Copy data from cpu-buffer --> gpu-buffer
    constexpr uint64_t RESOURCE_FLAG_GPU_READ = uint64_t(0x0000000000000004);        // Shader Resource View
    constexpr uint64_t RESOURCE_FLAG_GPU_WRITE = uint64_t(0x0000000000000008);        // RenderTarget or DepthStencil View

    constexpr uint64_t RESOURCE_FLAG_UAV = uint64_t(0x0000000000000100);        // Unordered Access View
    constexpr uint64_t RESOURCE_FLAG_GENERATE_MIPS = uint64_t(0x0000000000000200);        // Generate Mips
    constexpr uint64_t RESOURCE_FLAG_GPU_STRUCTURED = uint64_t(0x0000000000000400);        // HLSL: StructuredBuffer<xxx>
    constexpr uint64_t RESOURCE_FLAG_DRAW_INDIRECT_ARGS = uint64_t(0x0000000000001000);        // Used in Indirect Draw's
    constexpr uint64_t RESOURCE_FLAG_RAW = uint64_t(0x0000000000002000);        // HLSL: RWByteAddressBuffer
    constexpr uint64_t RESOURCE_FLAG_APPEND = uint64_t(0x0000000000004000);        // HLSL: AppendStructuredBuffer<xxx> 
    constexpr uint64_t RESOURCE_FLAG_COUNTER = uint64_t(0x0000000000008000);        // HLSL: ConsumeStructuredBuffer<xxx> 

    enum class EResourceType {
        BUFFER,
        TEXTURE,
        RENDER_TARGET,
        DEPTH_STENCIL
    };

    class RHICommandList;
    class RHIResource 
    {
    public:
        enum class ERHIResourceType 
        { 
            Buffer, 
            Texture, 
            Sampler, 
            AS 
        };
        virtual ~RHIResource() = default;

        virtual ERHIResourceType GetType() const = 0;
        virtual size_t GetSize() const = 0;

        // 内存映射（仅对CPU可见资源有效）
        virtual void* Map(size_t offset, size_t size) = 0;
        virtual void Unmap() = 0;

        // 获取底层资源句柄
        virtual void* GetNativeResource() = 0; // ID3D12Resource* 或 VkBuffer/VkImage

        // 状态转换
        virtual void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) = 0;

    };

    class RHIBuffer : public RHIResource
    {
    public:
        size_t GetSize() const override { return m_iSize; }

    protected:
        size_t  m_iSize;
    };

    class RHIAccelerationStructure : public RHIResource
    { 
       
    };


};