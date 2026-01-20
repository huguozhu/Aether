export module Aether:D3D12Shader;
import :RHIShader;
import :D3D12Definition;

export namespace Aether
{
    class D3D12Shader : public RHIShader
    {
    public:
        D3D12Shader(AetherEngine* context, EShaderStage stage, std::string const& name, std::string const& entry_func_name, std::string const& code)
            : RHIShader(context, stage, name, entry_func_name, code)
        {
        }

        AResult                 Active();
        void                    Deactive();
        D3D12_SHADER_BYTECODE   GetD3D12Bytecode() { return {}; }
    };

};
