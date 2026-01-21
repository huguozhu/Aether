module Aether:ShapeMeshComponent;
import :ShapeMeshComponent;
import :Engine;
import :EngineDefinition;
import :ShapeMesh;
import :RHIResource;
import :RHIMesh;

namespace Aether
{
    /******************************************************************************
     * CubeMeshComponent
     ******************************************************************************/
    CubeMeshComponent::CubeMeshComponent(AetherEngine* engine)
        :ShapeMeshComponent(engine)
    {
        this->SetName("Cube Mesh Component");

        // Create Mesh
        CreateCube(m_sMeshData);
        RHIMeshPtr pMesh = CreateMeshFromMeshData(m_pEngine, m_sMeshData);

        this->AddMesh(pMesh);
        this->SetAABBox(pMesh->GetAABBox());
    }

    /******************************************************************************
     * SphereMeshComponent
     ******************************************************************************/
    SphereMeshComponent::SphereMeshComponent(AetherEngine* engine, uint32_t x_segment_num, uint32_t y_segment_num)
        :ShapeMeshComponent(engine)
    {
        this->SetName("Sphere Mesh Component");

        // Create Mesh
        CreateSphere(m_sMeshData);
        RHIMeshPtr pMesh = CreateMeshFromMeshData(m_pEngine, m_sMeshData);

        this->AddMesh(pMesh);
        this->SetAABBox(pMesh->GetAABBox());
    }

    /******************************************************************************
     * ConeMeshComponent
     ******************************************************************************/
    ConeMeshComponent::ConeMeshComponent(AetherEngine* engine)
        :ShapeMeshComponent(engine)
    {
        this->SetName("Cone Mesh Component");

        // Create Mesh
        CreateCone(m_sMeshData);
        RHIMeshPtr pMesh = CreateMeshFromMeshData(m_pEngine, m_sMeshData);

        this->AddMesh(pMesh);
        this->SetAABBox(pMesh->GetAABBox());
    }


    /******************************************************************************
     * TerrainMeshComponent
     ******************************************************************************/
    TerrainMeshComponent::TerrainMeshComponent(AetherEngine* engine, float width, float height, uint32_t slices_x, uint32_t slices_z, float max_texcoord_u, float max_texcoord_v,
        const std::function<float(float, float)>& heightFunc, const std::function<float3(float, float)>& normalFunc)
        :ShapeMeshComponent(engine)
    {
        this->SetName("Terrain Mesh Component");

        // Create Mesh
        CreateTerrain(m_sMeshData, width, height, slices_x, slices_z, max_texcoord_u, max_texcoord_v, heightFunc, normalFunc);
        RHIMeshPtr pMesh = CreateMeshFromMeshData(m_pEngine, m_sMeshData);

        this->AddMesh(pMesh);
        this->SetAABBox(pMesh->GetAABBox());
    }

    /******************************************************************************
     * PlaneMeshComponent
     ******************************************************************************/
    PlaneMeshComponent::PlaneMeshComponent(AetherEngine* engine, float width, float height)
        :TerrainMeshComponent(engine, width, height, 1, 1)
    {
        this->SetName("Plane Mesh Component");
    }

};
