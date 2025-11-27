export module Aether.Utils:zbase64;
import :error;
import <string>;

export namespace Aether
{
    class ZBase64
    {
    public:
        static std::string Encode(const char* Data, int DataByte);
        static std::string Decode(const char* Data, int DataByte, int& OutByte);

        static SResult Encode(const char* Data, int DataByte, char** out);
        static SResult Decode(const char* Data, int DataByte, char** out);
    };

};