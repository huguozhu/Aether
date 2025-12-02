export module Aether.RHI:RHIResource;

export namespace Aether
{
    class RHIResource 
    {
    public:
        enum Type { Buffer, Texture, Sampler, AS };
        virtual ~RHIResource() = default;
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