export module Aether:SceneManager;

import :EngineDefinition;
import :Component;
import :Rect;
import :AABBox;
import :Log;

export namespace Aether
{
    using MeshPair = std::pair<MeshComponent*, uint32_t>;
    enum class RenderScope : uint32_t
    {
        Opacity = 1UL << 1,
        Transparent = 1UL << 2,
        ALL = 0xFFFFFFFF
    };

    class SceneManager
    {
    public:
        SceneManager(AetherEngine* engine);
        virtual                             ~SceneManager() = default;

        static void                         PrintSceneTree(SceneComponent* component, int level = 0);
        void                                PrintTree() const;

        SceneComponentPtr                   GetRootComponent() const { return m_pRootComponent; }

        bool                                IsSceneDirty() const { return m_bSceneDirty; }
        void                                SetSceneDirty(bool v) { m_bSceneDirty = v; }

        CameraComponent*                    GetActiveCamera();
        void                                SetActiveCamera(CameraComponent* cam) { m_pActiveCamera = cam; }

        int32_t                             GetActiveLightIndex() { return m_iActiveLightIndex; }
        void                                SetActiveLightIndex(int32_t index) { m_iActiveLightIndex = index; }

        size_t                              NumLightComponent() const { return m_vLightList.size(); }
        LightComponent*                     GetLightComponentByIndex(size_t index);

        SkyBoxComponent*                    GetSkyBoxComponent() { return m_pSkyBoxComponent; }
        std::vector<ParticleComponent*>&    GetParticleComponents() { return m_vParticleComponents; }

        virtual AResult                     Tick(float delta_time);


        Rect<uint32_t>                      GetRenderRect() const { return m_RenderRect; }

        //RHIBufferPtr&                    GetLightInfoCBuffer();
        //RHIBufferPtr&                    GetViewInfoCBuffer();

        const std::vector<MeshPair>&        GetOpaqueMeshList() { return m_OpaqueMeshList; }
        const std::vector<MeshPair>&        GetTransparentMeshList() { return m_TransparentMeshList; }

        //AResult							    RenderSprite2DScene();
        //void                                AddSprite2DComponent(Sprite2DComponentPtr const& sprite2d_component);
        //CameraComponentPtr                  GetSprite2DCamera(uint32_t w, uint32_t h);

        template<typename Query>
        std::vector<MeshPair> QueryMesh(Query query)
        {
            std::vector<MeshPair> outMeshes;
            CameraComponent* pActiveCamera = GetActiveCamera();
            if (!pActiveCamera)
                return outMeshes;

            std::vector<MeshPair>& allMeshes = m_mCachedVisibleMeshListByCamera[pActiveCamera];
            for (auto& mesh : allMeshes)
            {
                bool filter = query(mesh);
                if (filter)
                {
                    outMeshes.emplace_back(mesh);
                }
            }
            return outMeshes;
        }

        const std::vector<MeshPair>& GetVisableMeshes()
        {
            return m_mCachedVisibleMeshListByCamera[GetActiveCamera()];
        }

    protected:        
        void                                AddToEntityRecursion(SceneComponentPtr scene_component);

        void                                UpdateSkeletonMatrics();
        void                                ClipScene(CameraComponent* camera);
        void                                SortMeshList(CameraComponent* camera, std::vector<MeshPair>& opacity_list, std::vector<MeshPair>& transparent_list);

    protected:
        AetherEngine* m_pEngine = nullptr;

        Rect<uint32_t>                      m_RenderRect = Rect<uint32_t>(0, 0, 0, 0);
        /* *********************************************************************
         * Scene 3D
         * *********************************************************************/
        std::vector<LightComponentPtr>                      m_vDefaultLights;

        bool                                                m_bSceneDirty = false;
        SceneComponentPtr                                   m_pRootComponent = nullptr;

        // 3D Scene
        std::vector<Entity*>                                m_vCurEntities;
        std::vector<CameraComponent*>                       m_vCameraList;
        std::vector<LightComponent*>                        m_vLightList;
        SkyBoxComponent*                                    m_pSkyBoxComponent = nullptr;
        std::vector<ParticleComponent*>                     m_vParticleComponents;
        CameraComponent*                                    m_pActiveCamera = nullptr;
        int32_t                                             m_iActiveLightIndex = -1;

        std::vector<MeshComponent*>                         m_vMeshComponentList;
        std::vector<MeshPair>                               m_vMeshList;
        std::map<CameraComponent*, std::vector<MeshPair>>   m_mCachedVisibleMeshListByCamera;

        // 2D Scene
        std::vector<Sprite2DComponentPtr>                   m_vSprite2DComponentList;
        std::map<uint32_t, CameraComponentPtr>              m_vSprite2DCameraList;


        //RHIBufferPtr m_LightInfoCBuffer;
        //RHIBufferPtr m_ViewInfoCBuffer;

        std::vector<MeshPair> m_OpaqueMeshList; // meshes need to draw in current rendering loop, invalid after rendering
        std::vector<MeshPair> m_TransparentMeshList;
    };

};
