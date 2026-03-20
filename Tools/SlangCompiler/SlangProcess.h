#include "slang.h"
#include "slang-com-helper.h"
#include "slang-com-ptr.h"
#include <vector>
#include <string>

class SlangProcess
{
public:
    SlangResult Init();

    void DiagnoseIfNeeded(slang::IBlob* diagnosticsBlob);


    bool SetTargetNames(std::vector<std::string> targetNames);


public:
    Slang::ComPtr<slang::IGlobalSession> m_pGlobalSession = nullptr;

    std::vector<slang::TargetDesc> m_vTargetDescs;
};
