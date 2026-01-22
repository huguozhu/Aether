export module Aether:ForwardShadingRenderer;
import :SceneRenderer;
import :Engine;
import :EngineDefinition;

export namespace Aether
{
    class ForwardShadingRenderer : public SceneRenderer
    {
    public:
        ForwardShadingRenderer(AetherEngine* engine);
        virtual ~ForwardShadingRenderer() = default;

        virtual AResult     Init() override;
        virtual AResult     BuildRenderJobList() override;
         void    AppendShadowMapJobs(uint32_t light_index);

        // Rendering Jobs
        ERendererReturnValue RenderSceneJob();
        ERendererReturnValue RenderSkyBoxJob();


    protected:
        RHITexturePtr       m_pRenderSceneColorTex;
        RHITexturePtr       m_pRenderSceneDepthTex;
    };
};
