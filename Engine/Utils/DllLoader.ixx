export module Aether:DllLoader;

import <string>;

namespace Aether
{
    struct DllLoader
    {
        DllLoader(const std::string& _dllname)
            : dllname(_dllname)
        {
        }

        virtual ~DllLoader()
        {
            Unload();
        }

        bool    Load();
        void    Unload();
        bool    IsLoaded() { return module != nullptr; }
        void* FindSymbol(const std::string& funcname);

        void* module = nullptr;
        std::string dllname;
    };

};
