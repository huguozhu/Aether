export module Aether:MeshComponent;
import :SceneComponent;
import :AABBox;
import :Engine;
import :EngineDefinition;

export namespace Aether
{
    enum class VisibleMark : uint8_t
    {
        Yes,
        No,
        Partial,
    };

    class MeshComponent : public SceneComponent
    {
    public:
        MeshComponent(AetherEngine* engine);
        virtual ~MeshComponent() = default;

        void                        AddMesh(RHIMeshPtr mesh) { m_vMeshes.push_back(mesh); }
        void                        InsertMesh(size_t idx, RHIMeshPtr mesh) { m_vMeshes.insert(m_vMeshes.begin() + idx, mesh); }
        void                        DelMesh(RHIMeshPtr mesh);
        size_t                      NumMeshes() const { return m_vMeshes.size(); }
        RHIMeshPtr                  GetMeshByIndex(size_t index);
        std::vector<RHIMeshPtr>&    GetMeshes() { return m_vMeshes; }

        void                        SetVisible(bool b);

        VisibleMark                 GetVisibleMark() const { return m_eVisibleMark; }
        void                        SetVisibleMark(VisibleMark mark) { m_eVisibleMark = mark; }

        void                        SetAABBox(AABBox const& box) { m_cAABBox = box; }
        AABBox const& GetAABBox() const { return m_cAABBox; }
        void                        SetAABBoxWorld(AABBox const& box) { m_cAABBoxWorld = box; }
        AABBox const& GetAABBoxWorld() const { return m_cAABBoxWorld; }

        virtual AResult             OnRenderBegin();
        virtual AResult             OnRenderEnd();
        virtual AResult             Render();
        virtual AResult             RenderMesh(uint32_t i);

        bool                        IsInstanceMesh() { return m_bIsInstance; }


    protected:
        std::vector<RHIMeshPtr>     m_vMeshes;

        AABBox                      m_cAABBox;
        AABBox                      m_cAABBoxWorld;
        VisibleMark                 m_eVisibleMark = VisibleMark::Yes;
        bool                        m_bIsInstance = false;

        RHIBufferPtr                m_ModelInfoCBuffer;
        RHIBufferPtr                m_GenCubeShaodowCBuffer;
    };

};
