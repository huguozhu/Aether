import Aether.SampleCommon;
import Aether.Math;
import Aether.Kernel;
import Aether.RHI;

using namespace Aether;

class Tutorial : public AppFramework
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
    return A_Success;
}

int main()
{
    Tutorial app;
    return APP_RUN(&app);
}