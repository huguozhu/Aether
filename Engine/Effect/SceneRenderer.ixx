export module Aether:SceneRenderer;
import :Engine;
import :EngineDefinition;
import :SceneManager;
import :RHIViewport;

export namespace Aether
{
//#define BEGIN_TIMEQUERY(time_query) \
//    if (m_pContext->EnableProfile()) \
//        m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&SceneRenderer::BeginTimeQueryJob, this, time_query)));
//
//#define END_TIMEQUERY(time_query) \
//    if (m_pContext->EnableProfile()) \
//        m_vRenderingJobs.push_back(MakeUniquePtr<RenderingJob>(std::bind(&SceneRenderer::EndTimeQueryJob, this, time_query)));

    enum ERendererReturnValue : uint32_t
    {
        RRV_NextJob = 0x00000001 << 0,
        RRV_Finish = 0x00000001 << 1,
    };

    enum class ERenderStage : uint8_t
    {
        None,
        Sprite2D,

        PreZ,
        GenerateShadowMap,
        GenerateCubeShadowMap,
        GenerateCascadedShadowMap,
        GenerateReflectiveShadowMap,
        GenerateGBuffer,
        RenderScene,
    };

    using RenderingFunc = std::function<ERendererReturnValue()>;

    class RenderingJob
    {
    public:
        RenderingJob(RenderingFunc const& func) : m_pFunc(func) {}
        RenderingJob(RenderingFunc&& func)
            : m_pFunc(std::forward<RenderingFunc>(func)) {
        }
        ERendererReturnValue Run() { return m_pFunc(); }
    private:
        RenderingFunc m_pFunc;
    };

    struct LightInfo;
    struct CameraInfo;
    class SceneRenderer
    {
    public:
        virtual ~SceneRenderer() = default;

        virtual AResult         Init();
        virtual AResult         BuildRenderJobList() = 0;
        virtual bool            IsNeedShaderInvariant(ERenderStage stage) { return false; }

        // call after BuildRenderJobList
        bool                    HasRenderJob();
        ERendererReturnValue    DoRenderJob();

        //ERendererReturnValue    ToneMappingJob();
        ERendererReturnValue    FinishJob();

        ERendererType           GetRendererType() { return m_eRendererType; }
        ERenderStage            GetCurRenderStage() const { return m_eCurRenderStage; }
        void                    SetCurRenderStage(ERenderStage stage) { m_eCurRenderStage = stage; }

        //ShadowLayerPtr          GetShadowLayer() { return m_pShadowLayer; }
        bool                    IsNowShadowStage();
        //AResult                 FillLightInfoByLightIndex(LightInfo& info, CameraComponent* pCamera, size_t light_index);
        virtual AResult         RenderScene(uint32_t scope = (uint32_t)RenderScope::ALL);

        void SetViewport(const RHIViewport& viewport)
        {
            if (!m_FinalViewport.IsSameSize(viewport))
            {
                m_bRenderSizeChanged = true;
                m_RenderViewport.width = viewport.width;
                m_RenderViewport.height = viewport.height;
            }
            m_FinalViewport = viewport;
        }

    protected:
        SceneRenderer(AetherEngine* engine);
        void                    CreateRenderMeshes();

    protected:
        AetherEngine* m_pEngine = nullptr;
        ERendererType                                   m_eRendererType = ERendererType::Unknown;

        ERenderStage                                    m_eCurRenderStage = ERenderStage::None;
        std::vector<RenderingJobPtrUnique>              m_vRenderingJobs;

        // ToneMapping
        //PostProcessPtr                                  m_pToneMappingPostProcess = nullptr;

        //Viewport m_viewport;
        bool m_bRenderSizeChanged = false;

        RHIViewport m_RenderViewport;  // for intermediate resources during rendering, left/top are always 0
        RHIViewport m_FinalViewport;   // for user provided render target, draw on the specialed area

        std::vector<MeshPair> m_renderableMeshes;

        // ShadowMap & Cascaded ShadowMap
        //ShadowLayerPtr                                  m_pShadowLayer = nullptr;
    };

};
