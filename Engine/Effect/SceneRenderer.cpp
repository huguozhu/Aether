export module Aether:SceneRenderer;
import :SceneRenderer;
import :Engine;
import :EngineDefinition;
import :SceneManager;
import :RHIViewport;
import :Effect;
import :CameraComponent;
import :LightComponent;
import :RHIMesh;
import :Log;
import std;

export namespace Aether
{
    SceneRenderer::SceneRenderer(AetherEngine* engine)
        :m_pEngine(engine)
    {
    }

    AResult SceneRenderer::Init()
    {
        AResult ret = A_Success;

        return ret;
    }

    bool SceneRenderer::HasRenderJob()
    {
        return !m_vRenderingJobs.empty();
    }

    ERendererReturnValue SceneRenderer::DoRenderJob()
    {
        ERendererReturnValue rrv = RRV_NextJob;
        for (auto& job : m_vRenderingJobs)
        {
            rrv = job->Run();
            if (rrv != RRV_NextJob)
                break;
        }
        return rrv;
    }
    //ERendererReturnValue SceneRenderer::ToneMappingJob()
    //{
    //    m_eCurRenderStage = ERenderStage::None;
    //    RHIContext& rc = m_pEngine->RHIContextInstance();

    //    AResult ret = m_pToneMappingPostProcess->Run();
    //    if (ret != A_Success)
    //        LOG_ERROR_PRIERR(ret, "SceneRenderer::ToneMappingJob() m_pToneMappingPostProcess->Run() failed.");

    //    return RRV_NextJob;
    //}

    ERendererReturnValue SceneRenderer::FinishJob()
    {
        m_eCurRenderStage = ERenderStage::None;
        m_bRenderSizeChanged = false;
        return RRV_Finish;
    }
    //ERendererReturnValue SceneRenderer::BeginTimeQueryJob(RHITimeQueryPtr tq)
    //{
    //    if (tq)
    //        tq->Begin();
    //    return RRV_NextJob;
    //}
    //ERendererReturnValue SceneRenderer::EndTimeQueryJob(RHITimeQueryPtr tq)
    //{
    //    if (tq)
    //        tq->End();
    //    return RRV_NextJob;
    //}
    bool SceneRenderer::IsNowShadowStage()
    {
        if (m_eCurRenderStage == ERenderStage::GenerateShadowMap ||
            m_eCurRenderStage == ERenderStage::GenerateCubeShadowMap ||
            m_eCurRenderStage == ERenderStage::GenerateCascadedShadowMap)
            return true;
        else
            return false;
    }
    /*AResult SceneRenderer::FillLightInfoByLightIndex(LightInfo& info, CameraComponent* pCamera, size_t light_index)
    {
        SceneManager& sm = m_pEngine->SceneManagerInstance();
        LightComponent* light = sm.GetLightComponentByIndex(light_index);
        if (!light || !pCamera)
            return ERR_INVALID_ARG;

        float exposure = 1.0;
        if (m_pContext->GetLightingMode() == LightingMode::PBR)
            exposure = pCamera->GetExposure() * PBR_INTENSITY_COEFF;

        LightType type = light->GetLightType();
        info.color = light->GetColor().ToFloat3();
        info.type = (int)type;
        info.direction = light->GetDirection();
        info.falloffRadius = light->GetFalloffRadius();
        info.posWorld = light->GetLightPos();
        info.intensity = light->GetIntensity() * exposure;
        info.castShadow = (int)light->CastShadow();
        info.useSoftShadow = (int)light->SoftShadow();
        info.shadowBias = light->GetShadowBias();
        if (light->GetShadowMapCamera())
        {
            CameraComponent* pCamera = light->GetShadowMapCamera();
            info.nearFarPlane = float2(pCamera->GetNearPlane(), pCamera->GetFarPlane());
        }
        if (info.castShadow)
            info.shadowMapIndex = m_pContext->SceneRendererInstance().GetShadowLayer()->GetShadowMapIndexByLightIndex(light_index);
        if (LightType::Spot == type)
        {
            float2 inOutCutoff = ((SpotLightComponent*)light)->GetInOutCutoff();
            info.inOutCutoff = float2(cos(inOutCutoff.x()), cos(inOutCutoff.y()));
        }

        if (LightType::Spot == type || LightType::Directional == type)
        {
            Matrix4 const& light_vp = light->GetShadowMapCamera()->GetViewProjMatrix();
            info.lightViewProj = light_vp.Transpose();
        }

        return A_Success;
    }
    */
    AResult SceneRenderer::RenderScene(uint32_t scope)
    {
        if (m_renderableMeshes.empty())
            return A_Success;

        // to delete these codes, repeat at MeshComponent::RenderMesh()
        //{
        //    for (auto& mesh : m_renderableMeshes)
        //    {
        //        Technique* tech = nullptr;
        //        RHIMeshPtr pMesh = mesh.first->GetMeshByIndex(mesh.second);
        //        if (!pMesh->GetTechnique())
        //        {
        //            // auto choose a technique
        //            GetEffectTechniqueToRender(pMesh, &tech);
        //            pMesh->SetTechnique(tech);
        //        }
        //    }
        //}

        //// the SceneManager can supply the sort method
        //float3 base = m_pContext->SceneManagerInstance().GetActiveCamera()->GetWorldTransform().GetTranslation();
        //std::sort(m_renderableMeshes.begin(), m_renderableMeshes.end(), [base](const MeshPair& m1, const MeshPair& m2)->bool {
        //    float dis1 = Math::Distance(base, m1.first->GetMeshByIndex(m1.second)->GetAABBoxWorld().Center());
        //    float dis2 = Math::Distance(base, m2.first->GetMeshByIndex(m2.second)->GetAABBoxWorld().Center());

        //    if (m1.first->GetMeshByIndex(m1.second)->GetMaterial()->alpha_mode == AlphaMode::Blend &&
        //        m2.first->GetMeshByIndex(m2.second)->GetMaterial()->alpha_mode == AlphaMode::Blend)
        //    {
        //        return dis1 > dis2;
        //    }
        //    else if (m1.first->GetMeshByIndex(m1.second)->GetMaterial()->alpha_mode == AlphaMode::Blend)
        //    {
        //        return false;
        //    }
        //    else if (m2.first->GetMeshByIndex(m2.second)->GetMaterial()->alpha_mode == AlphaMode::Blend)
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return dis1 < dis2;
        //    }
        //    });


        //for (MeshPair& mesh_id : m_renderableMeshes)
        //{
        //    AETHER_RETIF_FAIL(mesh_id.first->RenderMesh(mesh_id.second));
        //}
        return A_Success;
    }
};
