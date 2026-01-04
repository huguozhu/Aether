export module Aether:RHIResource;

export namespace Aether
{

    enum class ResourceType {
        BUFFER,
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_CUBE,
        RENDER_TARGET,
        DEPTH_STENCIL
    };

    class RHIResource 
    {
    public:
        enum Type { Buffer, Texture, Sampler, AS };
        virtual ~RHIResource() = default;

        virtual ResourceType GetType() const = 0;
        virtual size_t GetSize() const = 0;

        // 内存映射（仅对CPU可见资源有效）
        virtual void* Map(size_t offset, size_t size) = 0;
        virtual void Unmap() = 0;

        // 获取底层资源句柄
        virtual void* GetNativeResource() = 0; // ID3D12Resource* 或 VkBuffer/VkImage

        // 状态转换
        /*virtual void TransitionBarrier(ICommandBuffer* cmdBuffer,
            ResourceState from,
            ResourceState to) = 0;*/

    };

    class RHIBuffer : public RHIResource
    { 
        /* ... */ 
    };
    class RHITexture : public RHIResource 
    { 
        /* ... */ 
    };
    class RHIAccelerationStructure : public RHIResource
    { 
        /* ... */ 
    };


};