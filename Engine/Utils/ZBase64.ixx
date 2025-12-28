export module Aether:zbase64;
import :error;
import <string>;

export namespace Aether
{
    class ZBase64
    {
    public:
        static std::string Encode(const char* Data, int DataByte);
        static std::string Decode(const char* Data, int DataByte, int& OutByte);

        static AResult Encode(const char* Data, int DataByte, char** out);
        static AResult Decode(const char* Data, int DataByte, char** out);
    };

};