export module Aether:D3D12Shader;
import :RHIShader;
import :D3D12Definition;

export namespace Aether
{
    class D3D12Shader : public RHIShader
    {
    public:
        D3D12Shader(AetherEngine* context, EShaderStage stage, std::string const& name, std::string const& entry_func_name, const void* byteCode, size_t byteCodeSize)
            : RHIShader(context, stage, name, entry_func_name, byteCode, byteCodeSize)
        {
        }

        D3D12_SHADER_BYTECODE   GetD3D12Bytecode() { return {(const void*)m_vCode.data(), m_vCode.size() }; }
    };

};
