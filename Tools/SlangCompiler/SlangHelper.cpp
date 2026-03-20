#include "SlangHelper.h"

/* ShadingLanguage supoort dxil & spirv only*/
ID3D12ShaderReflection* GetReflectionFromDXIL(const void* dxilData, size_t dxilSize)
{
    HRESULT hr;

    // 1. 创建DXC工具
    IDxcUtils* utils = nullptr;
    DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils));

    // 2. 创建容器反射器
    IDxcContainerReflection* containerReflection = nullptr;
    DxcCreateInstance(CLSID_DxcContainerReflection, IID_PPV_ARGS(&containerReflection));

    // 3. 将DXIL数据加载到Blob中
    IDxcBlobEncoding* dxilBlob = nullptr;
    utils->CreateBlobFromPinned((const BYTE*)dxilData, (UINT32)dxilSize,
        CP_UTF8, &dxilBlob);

    // 4. 加载容器
    containerReflection->Load(dxilBlob);

    // 5. 找到反射部分（RDAT）
    UINT32 partIndex;
    hr = containerReflection->FindFirstPartKind(DXC_PART_REFLECTION_DATA, &partIndex);
    if (FAILED(hr)) {
        // 回退：查找着色器部分
        hr = containerReflection->FindFirstPartKind(DXC_PART_DXIL, &partIndex);
        if (FAILED(hr)) {
            dxilBlob->Release();
            containerReflection->Release();
            utils->Release();
            return nullptr;
        }
    }

    // 7. 从反射部分创建反射接口
    ID3D12ShaderReflection* reflection = nullptr;
    hr = containerReflection->GetPartReflection(partIndex, IID_PPV_ARGS(&reflection));

    // 清理临时对象
    dxilBlob->Release();
    containerReflection->Release();
    utils->Release();

    return reflection;
}
void ParseDxilReflectInfo(size_t size, const void* p_code, const std::string& stageName, ReflectInfo& reflectInfo)
{
    ID3D12ShaderReflection* pReflection = GetReflectionFromDXIL(p_code, size);
    if (!pReflection) return;

    reflectInfo.code_type = CodeType::ByteCode;
    reflectInfo.stage = stageName;
    reflectInfo.entry_point = "main";

    D3D12_SHADER_DESC desc;
    pReflection->GetDesc(&desc);

    // ConstantBuffer
    for (UINT i = 0; i < desc.BoundResources; i++)
    {
        D3D12_SHADER_INPUT_BIND_DESC bindDesc;
        pReflection->GetResourceBindingDesc(i, &bindDesc);
        ResourceInfo info;
        info.name = bindDesc.Name;
        info.binding = bindDesc.BindPoint;
        info.bindCount = bindDesc.BindCount;
        info.id = bindDesc.uID;
        info.space = bindDesc.Space;

        if (bindDesc.Type == D3D_SIT_CBUFFER)
        {
            info.type = ResourceType::ConstantBuffer;
            ID3D12ShaderReflectionConstantBuffer* pCb = pReflection->GetConstantBufferByName(bindDesc.Name);
            if (pCb)
            {
                D3D12_SHADER_BUFFER_DESC buffer_desc;
                pCb->GetDesc(&buffer_desc);
                info.size = buffer_desc.Size;
            }
        }
        else if (bindDesc.Type == D3D_SIT_TBUFFER)
        {
            info.type = ResourceType::Texture;
        }
        else if (bindDesc.Type == D3D_SIT_TEXTURE)
        {
            info.type = ResourceType::Texture;
        }
        else if (bindDesc.Type == D3D_SIT_SAMPLER)
        {
            info.type = ResourceType::Sampler;
        }
        reflectInfo.resources.push_back(info);
    }

    // Input
    for (uint32_t i = 0; i < desc.InputParameters; ++i)
    {
        D3D12_SIGNATURE_PARAMETER_DESC input_desc;
        pReflection->GetInputParameterDesc(i, &input_desc);
        SignatureParameter input;
        input.semantic = input_desc.SemanticName;
        input.semantic_index = input_desc.SemanticIndex;
        input.location = input_desc.Register;
        reflectInfo.input_signatures.push_back(input);
    }

    // Output
    for (uint32_t i = 0; i < desc.OutputParameters; ++i)
    {
        D3D12_SIGNATURE_PARAMETER_DESC output_desc;
        pReflection->GetOutputParameterDesc(i, &output_desc);
        SignatureParameter output;
        output.semantic = output_desc.SemanticName;
        output.semantic_index = output_desc.SemanticIndex;
        output.location = output_desc.Register;
        reflectInfo.output_signatures.push_back(output);
    }

    return;
}
static void ParseSpirvReflectInfo(size_t size, const void* p_code, const std::string& stageName, SlangCompileTarget sl, ReflectInfo& reflectInfo)
{
    reflectInfo.stage = stageName;
    reflectInfo.entry_point = "main";

    SpvReflectShaderModule module;
    SpvReflectResult hr = spvReflectCreateShaderModule(size, p_code, &module);
    reflectInfo.code_type = CodeType::ByteCode;
    reflectInfo.stage = stageName;
    reflectInfo.entry_point = module.entry_point_name;

    uint32_t c = module.descriptor_binding_count;
    for (uint32_t i = 0; i < c; ++i)
    {
        SpvReflectDescriptorBinding* desc = module.descriptor_bindings + i;
        ResourceInfo res_info;
        res_info.name = desc->name;
        res_info.binding = desc->binding;
        res_info.bindCount = 1;
        res_info.size = 0;

        if (desc->resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_CBV)
        {
            res_info.type = ResourceType::ConstantBuffer;
            res_info.size = desc->block.size;

            for (uint32_t j = 0; j < desc->block.member_count; j++)
            {
                SpvReflectBlockVariable* member = desc->block.members + j;
                for (uint32_t k = 0; k < member->member_count; k++)
                {
                    SpvReflectBlockVariable* variable = member->members + k;
                    variable = variable;
                }
                member = member;
            }
        }
        else if (desc->resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_SRV)
        {
            if (desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER ||
                desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_SAMPLED_IMAGE ||
                desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_IMAGE)
                res_info.type = ResourceType::Texture;
            else
                res_info.type = ResourceType::Buffer;
        }
        else if (desc->resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_UAV)
        {
            if (desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER ||
                desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER ||
                desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC ||
                desc->descriptor_type == SpvReflectDescriptorType::SPV_REFLECT_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC)
                res_info.type = ResourceType::RWBuffer;
            else
                res_info.type = ResourceType::RWTexture;
        }
        else if (desc->resource_type == SpvReflectResourceType::SPV_REFLECT_RESOURCE_FLAG_SAMPLER)
            res_info.type = ResourceType::Sampler;
        reflectInfo.resources.push_back(res_info);
    }


    c = module.input_variable_count;
    for (uint32_t i = 0; i < c; ++i)
    {
        SpvReflectInterfaceVariable* variable = module.input_variables[i];
        SignatureParameter input_param;
        input_param.semantic = variable->semantic;
        input_param.semantic_index = 0;
        input_param.location = variable->location;
        reflectInfo.input_signatures.push_back(input_param);
    }

    c = module.output_variable_count;
    for (uint32_t i = 0; i < c; ++i)
    {
        SpvReflectInterfaceVariable* variable = module.output_variables[i];
        SignatureParameter input_param;
        input_param.semantic = variable->semantic;
        input_param.semantic_index = 0;
        input_param.location = variable->location;
        reflectInfo.output_signatures.push_back(input_param);
    }
    hr = hr;
    return;
}