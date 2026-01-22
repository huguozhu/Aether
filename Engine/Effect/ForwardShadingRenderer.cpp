module Aether:ForwardShadingRenderer;
import :ForwardShadingRenderer;
import :SceneRenderer;
import :Engine;
import :EngineDefinition;
import :LightComponent;
import :CameraComponent;

namespace Aether
{
    ForwardShadingRenderer::ForwardShadingRenderer(AetherEngine* engine)
        : SceneRenderer(engine)
    {
        m_eRendererType = ERendererType::Forward;
    }

    AResult ForwardShadingRenderer::Init()
    {
        return SceneRenderer::Init();
    }
    AResult ForwardShadingRenderer::BuildRenderJobList()
    {
        m_vRenderingJobs.clear();

        //m_renderableMeshes = m_pEngine->SceneManagerInstance().QueryMesh([](const MeshPair& mesh)->bool {
        //    const auto& mesh_ = mesh.first->GetMeshByIndex(mesh.second);
        //    if (mesh_->IsVisible())
        //        return true;
        //    else
        //        return false;
        //    });
        SceneManager& sm = m_pEngine->SceneManagerInstance();
        if (m_renderableMeshes.empty() &&
            sm.GetSkyBoxComponent() == nullptr)
            ;// return A_Success;


        //if (sm.GetSkyBoxComponent())
        //    m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&ForwardShadingRenderer::RenderSkyBoxJob, this)));

        m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&ForwardShadingRenderer::RenderSceneJob, this)));

        //if (m_pEngine->IsHDR())
        //    m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&SceneRenderer::ToneMappingJob, this)));

        // the last job should be FinishJob()
        m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&SceneRenderer::FinishJob, this)));
        return A_Success;
    }

    ERendererReturnValue ForwardShadingRenderer::RenderSceneJob()
    {
        AResult res;
        m_eCurRenderStage = ERenderStage::RenderScene;
        //m_pEngine->RHIContextInstance().BeginRenderPass({ "RenderScene" , m_pRenderSceneFB.get() });
        this->RenderScene();
        //m_pEngine->RHIContextInstance().EndRenderPass();
        m_eCurRenderStage = ERenderStage::None;        
        return RRV_NextJob;
    }
    ERendererReturnValue ForwardShadingRenderer::RenderSkyBoxJob()
    {
        //m_eCurRenderStage = RenderStage::None;
        //m_pEngine->RHIContextInstance().BeginRenderPass({ "RenderSkybox", m_pRenderSceneFB.get() });
        //SkyBoxComponent* skybox = m_pContext->SceneManagerInstance().GetSkyBoxComponent();
        //if (skybox)
        //    skybox->Render();
        //m_pEngine->RHIContextInstance().EndRenderPass();
        return RRV_NextJob;
    }

    void ForwardShadingRenderer::AppendShadowMapJobs(uint32_t light_index)
    {
        /*SceneManager& sc = m_pEngine->SceneManagerInstance();
        LightComponent* pLight = sc.GetLightComponentByIndex(light_index);
        if (!pLight) return;

        LightType type = pLight->GetLightType();
        switch (type)
        {
        case LightType::Point:
        {
            m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&ShadowLayer::GenerateShadowMapJob, m_pShadowLayer.get(), light_index)));
            break;
        }
        case LightType::Directional:
        case LightType::Spot:
        {
            m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&ForwardShadowLayer::PostProcessShadowMapJob, (ForwardShadowLayer*)m_pShadowLayer.get(), light_index)));
            break;
        }
        case LightType::Ambient:
        case LightType::Unknown:
        case LightType::Num:
            break;
        }*/
        return;
    }
};
