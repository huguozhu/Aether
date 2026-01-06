module;
#if defined(AETHER_PLATFORM_WINDOWS)
#include <Windows.h>
#else
#endif

module Aether:DllLoader;

import :DllLoader;

namespace Aether
{
    bool DllLoader::Load()
    {
        if (module) return true;
#if defined(AETHER_PLATFORM_WINDOWS)
        module = (void*)LoadLibraryA(dllname.c_str());
        return module != nullptr;
#else
        return false;
#endif
    }

    void DllLoader::Unload()
    {
        if (module)
        {
#if defined(AETHER_PLATFORM_WINDOWS)
            FreeLibrary((HMODULE)module);
#else
#endif
        }
    }

    void* DllLoader::FindSymbol(const std::string& funcname)
    {
        if (!module) return nullptr;
#if defined(AETHER_PLATFORM_WINDOWS)
        return (void*)::GetProcAddress((HMODULE)module, funcname.c_str());
#else
        return nullptr;
#endif
    }

};