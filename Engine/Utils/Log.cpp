module;
#include <stdio.h>
#include <cstdarg>

module Aether:Log;
import :Log;
import std;

#if defined(AETHER_PLATFORM_WINDOWS)
#define PATH_SLASH '\\'
#else
#define PATH_SLASH '/'
#endif

#define __TO_STR(x) #x
#define TO_STR(x) __TO_STR(x)
#define __LINE_STR__ TO_STR(__LINE__)

constexpr const char* str_end(const char* str) { return *str ? str_end(str + 1) : str; }
constexpr bool str_slant(const char* str) { return *str == PATH_SLASH ? true : (*str ? str_slant(str + 1) : false); }
constexpr const char* r_slant(const char* str) { return *str == PATH_SLASH ? (str + 1) : r_slant(str - 1); }
constexpr const char* file_name(const char* str) { return str_slant(str) ? r_slant(str_end(str)) : str; }
constexpr int LOG_MAX_LEN = 512;

namespace Aether
{
    enum class TraceLevel
    {
        Error = 1,
        Warning = 2,
        Info = 3,
        Debug = 4,
    };
    typedef void(*FPTRACECALLBACK)(TraceLevel level, const char* szLog);
    typedef void(*FPMONITORCALLBACK)(const char* log);

    static void TraceCallback(TraceLevel level, const char* szLog)
    {
        switch (level)
        {
        case TraceLevel::Info:
            fprintf(stdout, "[Aether][INFO] %s\n", szLog);
            break;
        case TraceLevel::Warning:
            fprintf(stdout, "[Aether][WARNING] %s\n", szLog);
            break;
        case TraceLevel::Error:
            fprintf(stdout, "[Aether][ERROR] %s\n", szLog);
            break;
        default:
            fprintf(stdout, "[Aether][UNKNOW] %s\n", szLog);
            break;
        }
    }

    void LOG_DEBUG(const char* fmt, ...)
    {
        char buffer[LOG_MAX_LEN] = { 0 };
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buffer, LOG_MAX_LEN, fmt, args);
        va_end(args);

        TraceCallback(TraceLevel::Debug, buffer);
    }

    void LOG_INFO(const char* fmt, ...)
    {
        char buffer[LOG_MAX_LEN] = { 0 };
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buffer, LOG_MAX_LEN, fmt, args);
        va_end(args);

        TraceCallback(TraceLevel::Info, buffer);
    }

    void LOG_WARNING(const char* fmt, ...)
    {
        char buffer[LOG_MAX_LEN] = { 0 };
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buffer, LOG_MAX_LEN, fmt, args);
        va_end(args);

        TraceCallback(TraceLevel::Warning, buffer);
    }

    void LOG_ERROR(const char* fmt, ...)
    {
        char buffer[LOG_MAX_LEN] = { 0 };
        va_list args;
        va_start(args, fmt);
        vsprintf_s(buffer, LOG_MAX_LEN, fmt, args);
        va_end(args);

        TraceCallback(TraceLevel::Error, buffer);
    }

};