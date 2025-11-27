export module Aether.Utils:compression;

import Aether.Kernel;

export namespace Aether
{
    class Compression
    {
    public:
        static SResult Compress(char** dst, uint32_t& dst_len, void* src, uint32_t src_len);
        static SResult Decompress(char* dst, uint32_t& dst_len, void* src, uint32_t src_len);
    };
};
