#include "SlangProcess.h"
#include "SlangHelper.h"
#include <iostream>

// 辅助函数：打印编译诊断信息
void SlangProcess::DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob) 
{
    if (diagnosticsBlob && diagnosticsBlob->getBufferSize() > 0) {
        std::cerr << "Diagnostic: " << (const char*)diagnosticsBlob->getBufferPointer() << std::endl;
    }
}

SlangResult SlangProcess::Init()
{
    // 正确的函数调用：使用 slang_createGlobalSession
    SlangResult result = slang_createGlobalSession(SLANG_API_VERSION, m_pGlobalSession.writeRef());

    if (SLANG_FAILED(result)) 
    {
        std::cerr << "Failed to create global session." << std::endl;
        return result;
    }
    return result;
}

bool SlangProcess::SetTargetNames(std::vector<std::string> targetNames)
{
    m_vTargetDescs.resize(targetNames.size());
    for (size_t i = 0; i != targetNames.size(); i++)
    {
        m_vTargetDescs[i].format = ParseShadingLanguage(targetNames[i]);
        if (m_vTargetDescs[i].format == SLANG_TARGET_UNKNOWN)
        {
            std::cerr << "invalid target shading target: " << targetNames[i] << std::endl;
            return false;
        }
        m_vTargetDescs[i].profile = m_pGlobalSession->findProfile("sm_6_9");
    }
    return true;
}
