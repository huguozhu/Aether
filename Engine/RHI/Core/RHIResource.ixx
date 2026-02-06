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

    enum class ERHIResourceType
    {
        Buffer,
        Texture,
        Sampler,
        Num,
    };

    class RHICommandList;
    class RHIResource 
    {
    public:
        virtual ERHIResourceType GetType() const { return m_eType; }        
        
        virtual void* GetNativeResource() = 0; // ID3D12Resource* »ò VkBuffer/VkImage
        virtual void TransitionBarrier(RHICommandList* cmdList, EResourceState from, EResourceState to) = 0;

    protected:
        ERHIResourceType    m_eType = ERHIResourceType::Num;
    };

    class RHIBuffer : public RHIResource
    {
    public:
        RHIBuffer(size_t size, ResourceFlags flags, size_t structure_stride = 0)
            :m_iSize(size), m_Flags(flags), m_iStructureStride(structure_stride)
        {
            m_eType = ERHIResourceType::Buffer;
        }
        size_t GetSize() { return m_iSize; }
        virtual AResult Create(uint32_t dataSize, const void* data) = 0;

    protected:
        size_t          m_iSize = 0;
        size_t          m_iStructureStride;
        ResourceFlags   m_Flags = RESOURCE_FLAG_NONE;
    };

    class RHIAccelerationStructure : public RHIResource
    {        
    };


};