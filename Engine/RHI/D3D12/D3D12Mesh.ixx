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
        void UpdateStates();

    private:
        std::vector<D3D12_INPUT_ELEMENT_DESC> m_vVertexElems;
        std::vector<D3D12_VERTEX_BUFFER_VIEW> m_vVertexBufferViews;
        D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;

        size_t m_iPsoHashValue;
        D3D12_INDEX_BUFFER_STRIP_CUT_VALUE m_eIndexBufferCutValue;
        D3D12_PRIMITIVE_TOPOLOGY_TYPE m_ePrimitiveTopology;
    };

};
