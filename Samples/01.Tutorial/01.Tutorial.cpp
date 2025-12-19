import Aether.Math;
import Aether.Kernel;
import Aether.RHI;

using namespace Aether;

int main()
{
    ERHIType type = ERHIType::D3D12;

    AetherEngine* engine = new AetherEngine;
    EngineInitInfo info;
    engine->StartEngine(info);

    return 0;
}