export module Aether:Log;

namespace Aether
{
    void LOG_DEBUG(const char* fmt, ...);
    void LOG_INFO(const char* fmt, ...);
    void LOG_WARNING(const char* fmt, ...);
    void LOG_ERROR(const char* fmt, ...);
};

