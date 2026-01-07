import SampleCommon;
import Aether;


using namespace Aether;

class Tutorial : public SampleCommon::AppFramework
{
public:
    Tutorial();
    virtual AResult OnCreate();
    virtual AResult OnUpdate();
};
Tutorial::Tutorial()
    :AppFramework("Tutorial")
{

}
AResult Tutorial::OnCreate()
{
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