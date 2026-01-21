export module Aether:D3D12Mesh;
import :RHIMesh;
import :RHIResource;
import :RHIStruct;


export namespace Aether
{
    class D3D12Mesh : public RHIMesh
    {
    public:
        D3D12Mesh(AetherEngine* engine)
            :RHIMesh(engine)
        {
        }
        ~D3D12Mesh() = default;
    };

};
