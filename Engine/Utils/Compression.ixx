export module Aether.Utils:compression;

import Aether.Kernel;

export namespace Aether
{
    class Compression
    {
    public:
        static AResult Compress(char** dst, uint32_t& dst_len, void* src, uint32_t src_len);
        static AResult Decompress(char* dst, uint32_t& dst_len, void* src, uint32_t src_len);
    };
};
