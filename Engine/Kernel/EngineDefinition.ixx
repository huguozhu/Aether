export module Aether:EngineDefinition;
import <string>;
import std;

#define CLASS_PTR(class_name)        using class_name##Ptr       = std::shared_ptr<class_name>;
#define CLASS_PTR_UNIQUE(class_name) using class_name##PtrUnique = std::unique_ptr<class_name>;
#define CLASS_DECLARE(class_name) class class_name; CLASS_PTR(class_name); CLASS_PTR_UNIQUE(class_name);

export namespace Aether
{
    using AResult = uint32_t;
    constexpr AResult A_Success = 0;
    enum AETHER_ERR_CODE // max error code is 255
    {
        AETHER_ERR_UNKNOWN = 1,
        AETHER_ERR_INVALID_ARG = 2,
        AETHER_ERR_INVALID_INIT = 3,
        AETHER_ERR_INVALID_INVOKE_FLOW = 4,
        AETHER_ERR_INVALID_SHADER = 5,
        AETHER_ERR_INVALID_MODEL_FILE = 6,
        AETHER_ERR_INVALID_DATA = 7,
        AETHER_ERR_NOT_SUPPORT = 8,
        AETHER_ERR_NOT_IMPLEMENTED = 9,
        AETHER_ERR_NO_MEM = 10,
        AETHER_ERR_NO_DATA = 11,
        AETHER_ERR_FILE_NOT_FOUND = 12,
        AETHER_ERR_SYSTEM_ERROR = 13,

        AETHER_ERR_CODE_NUM,
    };

    // Kernel
    CLASS_DECLARE(AetherEngine);

    // Scene Manager
    CLASS_DECLARE(SceneManager);

    // Effect
    //CLASS_DECLARE(Effect);
    //CLASS_DECLARE(Technique);
    //CLASS_DECLARE(VirtualTechnique)
    //CLASS_DECLARE(SceneRenderer);
    //CLASS_DECLARE(EffectParam);
    //CLASS_DECLARE(PostProcess);
    //CLASS_DECLARE(HDRPostProcess);
    //CLASS_DECLARE(LDRPostProcess);
    //CLASS_DECLARE(ShadowLayer);
    //CLASS_DECLARE(TaaPostProcess);

    // Component
    CLASS_DECLARE(Entity);
    CLASS_DECLARE(Component);
    CLASS_DECLARE(SceneComponent);
    CLASS_DECLARE(CameraComponent);
    CLASS_DECLARE(ConeMeshComponent);
    CLASS_DECLARE(CsmCameraComponent);
    CLASS_DECLARE(CubeMeshComponent);
    CLASS_DECLARE(DirectionalLightComponent);
    CLASS_DECLARE(GaussianBlur);
    CLASS_DECLARE(GlobalIllumination);
    CLASS_DECLARE(ImageComponent);
    CLASS_DECLARE(LightComponent);
    CLASS_DECLARE(LiquidGlassComponent);
    CLASS_DECLARE(MeshComponent);
    CLASS_DECLARE(SpotLightComponent);
    CLASS_DECLARE(ParticleComponent);
    CLASS_DECLARE(PointLightComponent);
    CLASS_DECLARE(SkeletalMeshComponent);
    CLASS_DECLARE(SkyBoxComponent);
    CLASS_DECLARE(SpringSkeletonComponent);
    CLASS_DECLARE(SphereMeshComponent);
    CLASS_DECLARE(Sprite2DComponent);
    CLASS_DECLARE(TerrainMeshComponent);
    CLASS_DECLARE(PlaneMeshComponent);
    CLASS_DECLARE(WaterMarkComponent);

    CLASS_DECLARE(KeyFrame);
    CLASS_DECLARE(TransformKeyFrame);
    CLASS_DECLARE(MorphTargetKeyFrame);
    CLASS_DECLARE(AnimationTrack);
    CLASS_DECLARE(TransformAnimationTrack);
    CLASS_DECLARE(MorphTargetAnimationTrack);
    CLASS_DECLARE(AnimationComponent);

    // Resource
    //CLASS_DECLARE(ResourceManager)

        // RHI
    //CLASS_DECLARE(Material);
    CLASS_DECLARE(RHIContext);
    //CLASS_DECLARE(RHIDepthStencilView);
    CLASS_DECLARE(RHIFence);
    //CLASS_DECLARE(RHIFrameBuffer);
    //CLASS_DECLARE(RHIMesh);
    //CLASS_DECLARE(RHIGpuBuffer);

    //CLASS_DECLARE(RHIShaderResourceView);
    //CLASS_DECLARE(RHIUnorderedAccessView);

    //CLASS_DECLARE(RHITexture);
    //CLASS_DECLARE(RHISampler);
    //CLASS_DECLARE(RHIShader);
    //CLASS_DECLARE(RHIProgram);
    //CLASS_DECLARE(RHIRenderState);
    //CLASS_DECLARE(RHIRenderTargetView);
    //CLASS_DECLARE(RHITimeQuery);

    // Parallel
    CLASS_DECLARE(JobSystem);
    //CLASS_DECLARE(Thread);

    // utils
    CLASS_DECLARE(Buffer);
    CLASS_DECLARE(BitmapBuffer);
    CLASS_DECLARE(Timer);


    template <typename T, typename... Args>
    inline std::shared_ptr<T> MakeSharedPtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    inline std::unique_ptr<T> MakeUniquePtr(Args&& ... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...), std::default_delete<T>());
    }
};
