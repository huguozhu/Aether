export module Aether:RHIMesh;
import :RHIResource;
import :RHIStruct;
import :AABBox;

export namespace Aether
{
    class RHIMesh
    {
    public:
        EMeshTopologyType               GetTopologyType() const { return m_eTopoType; }
        void                            SetTopologyType(EMeshTopologyType t) { m_eTopoType = t; }

        // Index Streams
        bool                            IsUseIndices() const;
        uint32_t                        GetNumIndices() const;
        RHIBufferPtr const&             GetIndexBuffer();
        EIndexBufferType                GetIndexBufferType() const { return m_eIndexBufferType; }
        void                            SetIndexBuffer(RHIBufferPtr buffer, EIndexBufferType type);
        //void                            SetIndexBufferResource(std::shared_ptr<VertexIndicesResource>& indicesRes);

        // Vertex Streams
        uint32_t                        GetNumVertex() const;
        uint32_t                        NumVertexStream();
        VertexStream&                   GetVertexStreamByIndex(uint32_t i);
        std::vector<VertexStream>&      GetVertexStreams();
        void                            AddVertexStream(RHIBufferPtr render_buffer, uint32_t buffer_offset, uint32_t stride, VertexFormat format, EVertexElementUsage usage, uint32_t usage_index);
        void                            AddVertexStream(VertexStream& vs);
        void                            AddInstanceVertexStream(RHIBufferPtr render_buffer, uint32_t buffer_offset, uint32_t stride, VertexFormat format, EVertexElementUsage usage, uint32_t usage_index, uint32_t instance_count, uint32_t divisor);
        VertexStream*                   GetInstanceVertexStream(EVertexElementUsage instance_type, uint32_t usage_index);

        // AABBox
        void                            SetAABBox(AABBox const& box) { m_cAABBox = box; }
        AABBox const&                   GetAABBox() const { return m_cAABBox; }
        void                            SetAABBoxWorld(AABBox const& box) { m_cAABBoxWorld = box; }
        AABBox const&                   GetAABBoxWorld() const { return m_cAABBoxWorld; }

        // Is Draw
        void                            SetVisible(bool isVisible) { m_bIsVisible = isVisible; }
        bool                            IsVisible() const { return m_bIsVisible; }
    protected:
        RHIMesh(AetherEngine* engine) :m_pEngine(engine) {}
        virtual ~RHIMesh() {}

        AetherEngine*               m_pEngine = nullptr;

        // index buffers
        RHIBufferPtr                m_pIndexBuffer = nullptr;
        EIndexBufferType            m_eIndexBufferType = EIndexBufferType::UInt16;

        // vertex buffers
        EMeshTopologyType           m_eTopoType = EMeshTopologyType::Triangles;
        std::vector<VertexStream>   m_vVertexStreams;

        // AABBox
        AABBox                      m_cAABBox;
        AABBox                      m_cAABBoxWorld;

        // Whether rendering
        bool                        m_bIsVisible = true;
    };
};