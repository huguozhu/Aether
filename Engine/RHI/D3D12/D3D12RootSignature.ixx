export module Aether:D3D12RootSignature;
import :RHIRootSignature;
import :Engine;
import :EngineDefinition;
import :D3D12Definition;
import :D3D12Translate;
import <string>;


export namespace Aether
{
    class D3D12RootSignature : public RHIRootSignature
    {
    public:
        D3D12RootSignature(AetherEngine* engine, RHIRootSignatureDesc desc);

        uint32_t GetParameterCount() const override { return static_cast<uint32_t>(m_vRootParameters.size()); }
        void* GetNativeRootSignature() override { return (void*)m_pRootSignature.Get(); }

    private:
        bool CreateRootSignature();

    private:
        ID3D12RootSignaturePtr m_pRootSignature;

        std::vector<D3D12_ROOT_PARAMETER> m_vRootParameters;
        std::vector<D3D12_DESCRIPTOR_RANGE> m_vDescriptorRanges;
        std::vector<D3D12_STATIC_SAMPLER_DESC> m_vStaticSamplers;

    };
};