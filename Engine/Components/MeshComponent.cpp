module Aether:MeshComponent;
import :MeshComponent;
import :LightComponent;
import :RHIMesh;
import :Log;
import :Error;
import :Engine;
import :Technique;
import :SceneRenderer;

#include "Utils/Macros.h"
namespace Aether
{
    MeshComponent::MeshComponent(AetherEngine* engine)
        :SceneComponent(engine, "MeshComponent", ComponentType::Mesh)
    {
    }

    void MeshComponent::DelMesh(RHIMeshPtr mesh)
    {
        auto iter = std::find(m_vMeshes.begin(), m_vMeshes.end(), mesh);
        if (iter != m_vMeshes.end())
        {
            m_vMeshes.erase(iter);
        }
    }

    void MeshComponent::SetVisible(bool b)
    {
        for (auto& mesh : m_vMeshes)
            mesh->SetVisible(b);
    }

    RHIMeshPtr MeshComponent::GetMeshByIndex(size_t index)
    {
        if (index >= m_vMeshes.size())
        {
            LOG_ERROR("MeshComponent::GetMeshByIndex invalid arg");
            return nullptr;
        }
        return m_vMeshes[index];
    }

    AResult MeshComponent::OnRenderBegin()
    {       
        return A_Success;
    }
    AResult MeshComponent::OnRenderEnd()
    {
        for (auto mesh : m_vMeshes)
        {
        }
        return A_Success;
    }
    AResult MeshComponent::Render()
    {
        for (uint32_t i = 0; i < m_vMeshes.size(); i++)
        {
            AETHER_RETIF_FAIL(RenderMesh(i));
        }
        return A_Success;
    }
    AResult MeshComponent::RenderMesh(uint32_t i)
    {
        RHIMeshPtr pMesh = m_vMeshes[i];
        if (!pMesh->IsVisible())
            return A_Success;

        Technique* tech;
        AETHER_RETIF_FAIL(m_pEngine->SceneRendererInstance().GetEffectTechniqueToRender(pMesh, &tech));
        AETHER_RETIF_FAIL(this->OnRenderBegin());
        AETHER_RETIF_FAIL(tech->Render(pMesh));
        AETHER_RETIF_FAIL(this->OnRenderEnd());

        return A_Success;
    }

};
