export module Aether:ShapeMeshComponent;
import :MeshComponent;
import :Engine;
import :EngineDefinition;
import :ShapeMesh;


export namespace Aether
{
    class ShapeMeshComponent : public MeshComponent
    {
    public:
        ShapeMeshComponent(AetherEngine* engine)
            :MeshComponent(engine) {
        }
        ~ShapeMeshComponent() = default;

    protected:
        ShapeMesh::MeshData m_sMeshData;
    };

    /******************************************************************************
     * CubeMeshComponent
     ******************************************************************************/
    class CubeMeshComponent : public ShapeMeshComponent
    {
    public:
        CubeMeshComponent(AetherEngine* engine);
        virtual ~CubeMeshComponent() = default;
    };

    /******************************************************************************
     * SphereMeshComponent
     ******************************************************************************/
    class SphereMeshComponent : public ShapeMeshComponent
    {
    public:
        SphereMeshComponent(AetherEngine* engine, uint32_t x_segment_num = 64, uint32_t y_segment_num = 64);
        virtual ~SphereMeshComponent() = default;
    };


    /******************************************************************************
     * ConeMeshComponent
     ******************************************************************************/
    class ConeMeshComponent : public ShapeMeshComponent
    {
    public:
        ConeMeshComponent(AetherEngine* engine);
        virtual ~ConeMeshComponent() = default;
    };

    /******************************************************************************
     * TerrainMeshComponent
     ******************************************************************************/
    class TerrainMeshComponent : public ShapeMeshComponent
    {
    public:
        TerrainMeshComponent(AetherEngine* engine, float width, float height, uint32_t slices_x = 10, uint32_t slices_z = 10, float max_texcoord_u = 1.0f, float max_texcoord_v = 1.0f,
            const std::function<float(float, float)>& heightFunc = [](float x, float z) { return 0.0f; },
            const std::function<float3(float, float)>& normalFunc = [](float x, float z) { return float3(0.0f, 1.0f, 0.0f); });
        virtual ~TerrainMeshComponent() = default;
    };

    /******************************************************************************
     * PlaneMeshComponent
     ******************************************************************************/
    class PlaneMeshComponent : public TerrainMeshComponent
    {
    public:
        PlaneMeshComponent(AetherEngine* engine, float width, float height);
        virtual ~PlaneMeshComponent() = default;
    };

};
