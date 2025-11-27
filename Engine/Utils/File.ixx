export module Aether.Utils:file;
import Aether.Kernel;
import :utils;
import <vector>;
import <string>;
import std;

export namespace Aether
{
    inline FILE* aether_fopen(const char* utf8FilePath, const char* mode)
    {
        if (!utf8FilePath || !mode)
            return nullptr;
#   ifdef AETHER_PLATFORM_WINDOWS
        std::wstring utf16FilePath = utf8_to_wchar(utf8FilePath);
        std::wstring utf16mode = utf8_to_wchar(mode);
        return _wfopen(utf16FilePath.c_str(), utf16mode.c_str());
#   else
        return fopen(utf8FilePath, mode);
#   endif
    }


    inline SResult read_file_content(const char* filePath, const char* mode, std::vector<uint8_t>& content)
    {
        FILE* _file = fopen(filePath, mode);
        if (!_file)
            return ERR_FILE_NOT_FOUND;

        fseek(_file, 0, SEEK_END);
        auto _size = ftell(_file);
        fseek(_file, 0, SEEK_SET);
        content.resize(_size);
        fread((void*)content.data(), 1, _size, _file);
        fclose(_file);
        return S_Success;
    }
};
