module Aether:RHIShader;
import :RHIShader;

namespace Aether
{

    void RHIShader::AddParam(EffectParam* param)
    {
        m_vParams.push_back(param);
        m_bCompileReady = false;
    }
    void RHIShader::AddMacro(const std::string& name, const std::string& value)
    {
        m_vMacros[name] = value;
        m_bCompileReady = false;
    }

    void RHIShader::AddPredefine(const EffectPredefine& predefine)
    {
        m_vPredefines.push_back(predefine);
    }

    void RHIShader::SetCsThreadsPerGroup(uint32_t x, uint32_t y, uint32_t z)
    {
        m_iCsThreadsPerGroup = uint3(x, y, z);
    }
    void RHIShader::GetCsThreadsPerGroup(uint32_t& x, uint32_t& y, uint32_t& z)
    {
        x = m_iCsThreadsPerGroup[0];
        y = m_iCsThreadsPerGroup[1];
        z = m_iCsThreadsPerGroup[2];
    }
};
