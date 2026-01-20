module;
import <windows.h>;

module Aether:D3D12RootSignature;
import :D3D12RootSignature;
import :Engine;
import :EngineDefinition;
import :D3D12Definition;
import :D3D12Context;
import :D3D12Translate;
import :Utils;

namespace Aether
{
    D3D12RootSignature::D3D12RootSignature(AetherEngine* engine, RHIRootSignatureDesc desc)
        :RHIRootSignature(engine, desc)
    {
        this->CreateRootSignature();
        this->Hash();
    }

	bool D3D12RootSignature::CreateRootSignature()
	{
        D3D12Context* d3d_context = (D3D12Context*)(&m_pEngine->RHIContextInstance());        
        ID3D12Device* d3dDevice = d3d_context->GetD3D12Device();

        // 转换根参数
        for (const auto& param : m_Desc.parameters) 
        {
            D3D12_ROOT_PARAMETER rootParam = {};
            rootParam.ParameterType = D3D12Translate::TranslateParameterType(param.type);
            rootParam.ShaderVisibility = D3D12Translate::TranslateShaderVisibility(param.visibility);

            switch (param.type) 
            {
            case ERootParameterType::DescriptorTable:
            {
                std::vector<D3D12_DESCRIPTOR_RANGE> ranges;
                for (const auto& range : param.descriptorTable.ranges)
                {
                    D3D12_DESCRIPTOR_RANGE d3dRange = {};
                    d3dRange.RangeType = D3D12Translate::TranslateDescriptorRangeType(range.type);
                    d3dRange.NumDescriptors = range.numDescriptors;
                    d3dRange.BaseShaderRegister = range.baseShaderRegister;
                    d3dRange.RegisterSpace = range.registerSpace;
                    d3dRange.OffsetInDescriptorsFromTableStart = range.offsetInDescriptorsFromTableStart;
                    ranges.push_back(d3dRange);

                    // 保存到成员变量中以便生命周期管理
                    m_vDescriptorRanges.push_back(d3dRange);
                }

                rootParam.DescriptorTable.NumDescriptorRanges = static_cast<UINT>(ranges.size());
                rootParam.DescriptorTable.pDescriptorRanges = ranges.data();
                break;
            }

            case ERootParameterType::Constant32Bit: {
                rootParam.Constants.Num32BitValues = param.constants.num32BitValues;
                rootParam.Constants.ShaderRegister = param.constants.shaderRegister;
                rootParam.Constants.RegisterSpace = param.constants.registerSpace;
                break;
            }

            case ERootParameterType::ConstantBuffer:
            case ERootParameterType::ShaderResourceView:
            case ERootParameterType::UnorderedAccessView: {
                rootParam.Descriptor.ShaderRegister = param.descriptor.shaderRegister;
                rootParam.Descriptor.RegisterSpace = param.descriptor.registerSpace;
                break;
            }
            }

            m_vRootParameters.push_back(rootParam);
        }

        // 转换静态采样器
        for (const auto& sampler : m_Desc.staticSamplers) {
            D3D12_STATIC_SAMPLER_DESC staticSampler = {};
            staticSampler.Filter = D3D12Translate::TranslateFilter(sampler.filter);
            staticSampler.AddressU = D3D12Translate::TranslateAddressMode(sampler.addressU);
            staticSampler.AddressV = D3D12Translate::TranslateAddressMode(sampler.addressV);
            staticSampler.AddressW = D3D12Translate::TranslateAddressMode(sampler.addressW);
            staticSampler.MipLODBias = sampler.mipLODBias;
            staticSampler.MaxAnisotropy = sampler.maxAnisotropy;
            staticSampler.ComparisonFunc = D3D12Translate::TranslateCompareFunction(sampler.comparisonFunc);
            staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
            staticSampler.MinLOD = sampler.minLOD;
            staticSampler.MaxLOD = sampler.maxLOD;
            staticSampler.ShaderRegister = sampler.shaderRegister;
            staticSampler.RegisterSpace = sampler.registerSpace;
            staticSampler.ShaderVisibility = D3D12Translate::TranslateShaderVisibility(sampler.visibility);

            m_vStaticSamplers.push_back(staticSampler);
        }

        // 创建根签名描述
        D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
        rootSignatureDesc.NumParameters = static_cast<UINT>(m_vRootParameters.size());
        rootSignatureDesc.pParameters = m_vRootParameters.data();
        rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(m_vStaticSamplers.size());
        rootSignatureDesc.pStaticSamplers = m_vStaticSamplers.data();
        rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        // 序列化根签名
        ID3DBlobPtr signatureBlob;
        ID3DBlobPtr errorBlob;

        HRESULT hr = D3D12SerializeRootSignature(
            &rootSignatureDesc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &signatureBlob,
            &errorBlob
        );

        if (FAILED(hr)) {
            if (errorBlob) {
                LOG_ERROR("Failed to serialize root signature: {}",
                    static_cast<const char*>(errorBlob->GetBufferPointer()));
            }
            return false;
        }

        // 创建根签名
        hr = d3dDevice->CreateRootSignature(
            0,
            signatureBlob->GetBufferPointer(),
            signatureBlob->GetBufferSize(),
            IID_PPV_ARGS(&m_pRootSignature)
        );

        if (FAILED(hr)) {
            LOG_ERROR("Failed to create root signature: 0x{:X}", hr);
            return false;
        }

        // 设置名称
        if (!m_Desc.name.empty()) 
            m_pRootSignature->SetName(StringToWString(m_Desc.name.c_str()).c_str());

		return true;
	}



};