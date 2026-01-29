export module Aether:Technique;
import :Engine;
import :EngineDefinition;
import :RHIStruct;
import :Parameter;
import :ShaderHelper;
import std;

export namespace Aether
{
    class Technique;
    class VirtualTechnique
    {
    public:
        VirtualTechnique(AetherEngine* engine) : m_pEngine(engine)
        {
        }
        ~VirtualTechnique() = default;

        void SetName(const std::string& name) { m_techName = name; }
        const std::string& GetName() const { return m_techName; }

        void SetDefaultRenderState(const RHIRenderStateDesc& renderState) { m_defaultRenderState = renderState; }
        const RHIRenderStateDesc& GetDefaultRenderState() { return m_defaultRenderState; }

        void SetShaderName(EShaderStage shaderStage, const std::string& shaderName) { m_shaderNames[(uint32_t)shaderStage] = shaderName; }
        const std::vector<MetaPredefine>& GetMetaPredefines() const { return m_predefines; }

        bool IsGraphicsPipeline() const { return m_metaShaderResources[(size_t)EShaderStage::Vertex] != nullptr; }
        bool IsComputePipeline() const { return m_metaShaderResources[(size_t)EShaderStage::Compute] != nullptr; }

        AResult Build();
        Technique* Concrete(const std::vector<EffectPredefine>& predefines, const RHIRenderStateDesc& renderStateDesc);
        Technique* Concrete(const std::vector<EffectPredefine>& predefines);
        Technique* Concrete();

    private:
        AetherEngine* m_pEngine = nullptr;
        std::string m_techName;
        std::vector<MetaPredefine> m_predefines;
        RHIRenderStateDesc m_defaultRenderState{};
        std::array<std::string, (uint32_t)EShaderStage::Num> m_shaderNames; // shader names which is not actived
        std::array<MetaShaderResourcePtr, (uint32_t)EShaderStage::Num> m_metaShaderResources;

        std::unordered_map<std::string, TechniquePtr> m_concreteTechs;
    };

    class Technique
    {
    public:
        struct Param
        {
            using BindingArray = std::array<uint32_t, (uint32_t)EShaderStage::Num>;

            std::string name;
            std::string fallbackName;
            EffectDataType dataType = EffectDataType::Unknown;
            uint32_t arraySize = 1;
            BindingArray bindings; // uniform param can be shared between shaders, and can have different bindings

            // when dataType is SampledTexture, this is the original separate texture&sampler name
            std::string textureParamName;
            std::string samplerParamName;

            EffectVariablePtr variable;

            Param()
            {
                std::fill(bindings.begin(), bindings.end(), (uint32_t) - 1);
            }
        };
        using ParamMap = std::unordered_map<std::string, Param>;

    //    // no predefine
    //    // fixed-shader
    //    // params
    //    // vertex attributes
    //    // render state
    //    // pipeline state
        Technique(AetherEngine* engine, VirtualTechnique* virtualTechnique)
            : m_pEngine(engine), m_pVirtualTechnique(virtualTechnique)
        {
        }

        ~Technique() = default;

        VirtualTechnique* GetVirtualTechnique() const { return m_pVirtualTechnique; }

        void SetName(const std::string& name) { m_techName = name; }
        const std::string& GetName() const { return m_techName; }

        void SetShaderResource(EShaderStage shaderStage, const ShaderResourcePtr& shaderRes)
        {
            // TODO: check shaderStage is valid
            m_shaderRes[(uint32_t)(shaderStage)] = shaderRes;
        }

        const ShaderResourcePtr& GetShaderResource(EShaderStage shaderStage) const
        {
            // TODO: check shaderStage is valid
            return m_shaderRes[(uint32_t)(shaderStage)];
        }

        void SetRenderStateDesc(const RHIRenderStateDesc& renderStateDesc) { m_RenderStateDesc = renderStateDesc; }
        RHIRenderStateDesc& GetRenderStateDesc() { return m_RenderStateDesc; }

    //    RHIRenderStatePtr& GetRenderState();

        const ParamMap& GetFormalParams() const { return m_params; }

       template<typename T>
       void SetParam(const std::string& name, const T& value)
       {
           auto paramIt = m_params.find(name);
           if (paramIt == m_params.end())
               return;

           *(paramIt->second.variable) = value;
       }

        bool HasParam(const std::string& name) { return m_params.find(name) != m_params.end(); }

    //    AResult Render(const RHIMeshPtr& mesh);
    //    void Dispatch(uint32_t x, uint32_t y, uint32_t z);
    //    void DispatchIndirect(RHIBufferPtr indirectBuf);
    //    void DrawIndirect(RHIBufferPtr indirectBuf, EMeshTopologyType type);
    //    void DrawInstanced(EMeshTopologyType type, uint32_t vertexCountPerInstance, uint32_t instanceCount, uint32_t startVertexLocation, uint32_t startInstanceLocation);

    //    AResult Commit();
    //    void Uncommit();

    private:
        AResult Build();
        void CreateEffectVariable(Param& param);

    private:
        AetherEngine* m_pEngine = nullptr;
        VirtualTechnique* m_pVirtualTechnique;
        std::string m_techName;
        RHIRenderStateDesc m_RenderStateDesc;
    //    RHIRenderStatePtr m_RenderState;
    //    RHIRenderStatePtr m_RenderStateForTransparent;

        ParamMap m_params; // all params of this technique that user can set
    //    std::array<std::vector<size_t>, (uint32_t)EShaderStage::Num> m_shaderParamsIndex;

        std::array<ShaderResourcePtr, (uint32_t)EShaderStage::Num> m_shaderRes; // shader names which is actived
    

    //    bool m_bOpenGLAlreadyRemapBinding = false;

    private:
        friend class VirtualTechnique;
    };
};
