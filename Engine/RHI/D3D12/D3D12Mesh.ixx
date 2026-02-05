export module Aether:D3D12Mesh;
import :RHIMesh;
import :RHIResource;
import :RHIStruct;
import :D3D12Definition;


export namespace Aether
{
    class D3D12CommandList;
    class D3D12Mesh : public RHIMesh
    {
    public:
        D3D12Mesh(AetherEngine* engine)
            :RHIMesh(engine)
        {
        }
        ~D3D12Mesh() = default;

        AResult Active(D3D12CommandList* pCmd);
    };

};
