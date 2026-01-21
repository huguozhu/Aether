import SampleCommon;
import Aether;


using namespace Aether;

class Tutorial : public SampleCommon::AppFramework
{
public:
    Tutorial();
    virtual AResult OnCreate();
    virtual AResult OnUpdate();

private:
    EntityPtr m_pEntity = nullptr;

};


Tutorial::Tutorial()
    :AppFramework("Tutorial")
{

}
AResult Tutorial::OnCreate()
{
    RHIContext& rc = m_pEngine->RHIContextInstance();
    //Viewport const& vp = rc.GetScreenRHIFrameBuffer()->GetViewport();
    float w = 1080.0;// vp.width;
    float h = 1920.0;// vp.height;

    m_pEntity = MakeSharedPtr<Entity>(m_pEngine.get());
    m_pEntity->AddToTopScene();

    // Camera
    CameraComponentPtr pCam = MakeSharedPtr<CameraComponent>(m_pEngine.get());
    pCam->ProjPerspectiveParams(Math::PI / 4, w / h, 0.1f, 1000.0f);
    pCam->SetLookAt(float3(0, 2, 3), float3(0, 0, 0), float3(0, 1, 0));
    m_pEntity->AddSceneComponent(pCam);

    // Mesh
    ConeMeshComponentPtr pMesh = MakeSharedPtr<ConeMeshComponent>(m_pEngine.get());
    m_pEntity->AddSceneComponent(pMesh);

    return A_Success;
}
AResult Tutorial::OnUpdate()
{
    return m_pEngine->Update();
}

int main()
{
    Tutorial app;
    return APP_RUN(&app);
}