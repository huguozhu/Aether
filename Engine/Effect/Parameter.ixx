export module Aether:Parameter;
import :Engine;
import :EngineDefinition;
import :Variable;
import :Error;
import :RHIRenderState;
import :RHIStruct;

export namespace Aether
{
    /******************************************************************
     * Independent Function
     *****************************************************************/
    ETexFilterOp         TexFilterOpFromName(std::string const& name);
    ETexAddressMode      TexAddressingModeFromName(std::string const& name);
    ECompareFunction     CompareFunctionFromName(std::string const& name);
    ECullMode            CullModeFromName(std::string const& name);
    EBlendOperation      BlendOperationFromName(std::string const& name);
    EStencilOperation    StencilOperationFromName(std::string const& name);
    EBlendFactor         BlendFactorFromName(std::string const& name);
    bool                BoolFromString(std::string const& str);

    /******************************************************************************
     * EffectDataType
     ******************************************************************************/
    enum class EffectDataType : uint32_t
    {
        Unknown = 0,
        ConstantBuffer,
        Buffer,
        RWBuffer,
        Texture,
        RWTexture,
        Sampler,
        SampledTexture, // for some shading language has no separate texture, only have combined texture, like glsl
    };

    /******************************************************************************
     * EffectParam
     ******************************************************************************/
    struct EffectParam final
    {
        template <class T>
        EffectParam& operator=(T const& value)
        {
            *m_pVariable = value;
            return *this;
        }

        template <class T>
        void GetValue(T& val) const
        {
            m_pVariable->Value(val);
        }

        AResult UpdateConstantBuffer(const void* data, size_t size);

        std::string     const& GetName()               const { return m_szName; }
        EffectVariable  const& GetVar()                const { return *m_pVariable; }
        EffectDataType          GetDataType()          const { return m_eDataType; }

        std::unique_ptr<EffectVariable> ReadRenderVariable(EffectDataType data_type);

        std::string                                             m_szName;
        std::string                                             m_fallbackName;
        EffectDataType                                          m_eDataType = EffectDataType::Unknown;
        std::unique_ptr<EffectVariable>                         m_pVariable;
        std::map<std::string, uint32_t>                         m_vBindings;

        // valid when m_eDataType is EffectDataType::SampledTexture
        EffectParam* m_pTextureParam = nullptr;
        EffectParam* m_pSamplerParam = nullptr;
    };

    struct EffectPredefine
    {
        std::string name;
        std::string value;
    };

    std::vector<EffectPredefine> NULL_PREDEFINES;
    std::vector<std::string>     NULL_PARAMS;

    std::string GenerateSeedString(std::vector<EffectPredefine>& predefines);
};
