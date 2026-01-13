export module Aether:Effect;

#include "kernel/kernel.h"
#include "rhi/base/rhi_shader.h"
#include "effect/parameter.h"
#include "effect/technique.h"

export namespace Aether
{

    // manager all Technique resources and the related Shader resources
    class Effect
    {
    public:
        Effect(AetherEngine* engine)
            : m_pEngine(engine)
        {
        }

        ~Effect() = default;

        AResult Initialize();
        RHIShader* CreateShader(ShaderType stage, const ShaderResourcePtr& shaderRes);
        VirtualTechnique* GetVirtualTechnique(const std::string& name);
        Technique* GetTechnique(const std::string& name, const std::vector<EffectPredefine>& user_predefines);
        Technique* GetTechnique(const std::string& name);

        AResult LoadTechnique(const std::string& name, const RenderStateDesc* pDefaultRenderStateDesc,
            const char* vertexShaderName, const char* pixelShaderName, const char* computeShaderName = nullptr);

    private:
        void LoadDefaultVirtualTechniques();

    private:
        AetherEngine* m_pEngine = nullptr;
        std::unordered_map<std::string, RHIShaderPtr> m_Shaders;
        std::unordered_map<std::string, VirtualTechniquePtrUnique> m_VirtualTechniques;
    };

};
