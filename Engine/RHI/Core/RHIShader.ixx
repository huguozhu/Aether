export module Aether:RHIShader;
import :RHIStruct;
import :Vector;
import :ShaderHelper;
import :Parameter;
import std;

export namespace Aether
{    
    class EffectParam;
    class RHIShader
    {
    public:
        void SetShaderCode(const void* byteCode, size_t byteCodeSize)
        {
            m_szCode.resize(byteCodeSize);
            memcpy((void*)m_szCode.data(), byteCode, byteCodeSize);
        }

        void            AddParam(EffectParam* param);
        void            AddMacro(const std::string& name, const std::string& value);
        void            AddPredefine(const EffectPredefine& predefine);
        EShaderStage    Stage() const { return m_eShaderStage; }

        void            SetCodePrecompiled(bool bPrecompiled) { m_bCodePrecompiled = bPrecompiled; }

        void            SetCsThreadsPerGroup(uint32_t x, uint32_t y, uint32_t z) {}
        void            GetCsThreadsPerGroup(uint32_t& x, uint32_t& y, uint32_t& z) {}
        size_t          GetPredefineNum()
        {
            return m_vPredefines.size();
        }

        EffectPredefine& GetPredefineByIndex(size_t idx)
        {
            return m_vPredefines[idx];
        }

        std::vector<EffectPredefine>& GetPredefine()
        {
            return m_vPredefines;
        }


        std::string& GetCode()
        {
            return m_szCode;
        }

        std::string& GetEntryPoint()
        {
            return m_szEntryFuncName;
        }

        std::map<std::string, std::string>& GetMacros()
        {
            return m_vMacros;
        }

        const std::vector<EffectParam*>& GetParams()
        {
            return m_vParams;
        }

    protected:
        RHIShader(AetherEngine* engine)
            : m_pEngine(engine) {
        }
        RHIShader(AetherEngine* engine, EShaderStage stage, std::string const& name, std::string const& entry_func_name, std::string const& code)
            : m_pEngine(engine), m_eShaderStage(stage), m_szName(name), m_szEntryFuncName(entry_func_name), m_szCode(code) {
        }
        virtual ~RHIShader() {}

        virtual AResult   OnCompile() = 0;

    protected:
        AetherEngine* m_pEngine = nullptr;
        bool                                    m_bCompileReady = false;
        EShaderStage                            m_eShaderStage = EShaderStage::Num;
        std::string                             m_szName;
        std::string                             m_szEntryFuncName;
        std::string                             m_szCode;
        std::vector<EffectParam*>               m_vParams;
        std::vector<EffectPredefine>            m_vPredefines;
        std::map<std::string, std::string>      m_vMacros;

        uint3                                   m_iCsThreadsPerGroup = uint3(0, 0, 0);
        bool                                    m_bCodePrecompiled = false;
    };
};
