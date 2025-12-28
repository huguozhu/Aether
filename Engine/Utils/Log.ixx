module;
#include <cstdarg>

export module Aether:log;
import <string>;


#if defined(AETHER_PLATFORM_WINDOWS)
#define PATH_SLASH '\\'
#else
#define PATH_SLASH '/'
#endif

#define __TO_STR(x) #x
#define TO_STR(x) __TO_STR(x)
#define __LINE_STR__ TO_STR(__LINE__)

constexpr const char* str_end(const char* str)  { return *str ? str_end(str + 1) : str; }
constexpr bool str_slant(const char* str)       { return *str == PATH_SLASH ? true : (*str ? str_slant(str + 1) : false); }
constexpr const char* r_slant(const char* str)  { return *str == PATH_SLASH ? (str + 1) : r_slant(str - 1); }
constexpr const char* file_name(const char* str){ return str_slant(str) ? r_slant(str_end(str)) : str; }
constexpr int LOG_MAX_LEN = 512;

export namespace Aether
{
    inline void LOG_DEBUG(const char* fmt, ...)
    {
        char buffer[LOG_MAX_LEN] = { 0 };
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buffer, LOG_MAX_LEN, fmt, args);
        va_end(args);
    }

    inline void LOG_INFO(const char* fmt, ...)
    {
    }

    inline void LOG_WARNING(const char* fmt, ...)
    {
    }

    inline void LOG_ERROR(const char* fmt, ...)
    {
    }

    inline void LOG_ERROR_RET(const char* fmt, ...)
    {
    }

    inline void LOG_ERROR_PRIERR(const char* fmt, ...)
    {
    }
};
