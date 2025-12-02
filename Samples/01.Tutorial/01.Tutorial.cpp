import Aether.Math;
import Aether.Kernel;
import Aether.RHI;

using namespace Aether;

int main()
{
    ERHIType type = ERHIType::D3D12;

    AetherEngine* engine = new AetherEngine();
    engine->StartEngine("init.json");

    return 0;
}