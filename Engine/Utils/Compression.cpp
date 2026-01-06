module;
#include "zlib.h"

module Aether:Compression;
import :Compression;
import :Log;
import :Error;

namespace Aether
{
    AResult Compression::Compress(char** dst, uint32_t& dst_len, void* src, uint32_t src_len)
    {
        if (!src || src_len <= 0)
        {
            LOG_ERROR("Compression::Compress, invalid src args");
            return ERR_INVALID_ARG;
        }

        uLong uSrcLen = static_cast<uLong>(src_len);
        const Bytef* pSrc = reinterpret_cast<const Bytef*>(src);
        Bytef* pDst = NULL;

        uLong uDstLen = compressBound(uSrcLen);
        if (uDstLen < 0)
        {
            LOG_ERROR("Compression::Compress, invalid dest buffer length bound %d", uDstLen);
            return ERR_INVALID_ARG;
        }

        pDst = (Bytef*)malloc(uDstLen);
        if (pDst == NULL)
        {
            LOG_ERROR("Compression::Compress, memory alloc failed");
            return ERR_NO_MEM;
        }
        if (Z_OK != compress2(pDst, &uDstLen, pSrc, uSrcLen, Z_BEST_COMPRESSION))
        {
            free(pDst);
            LOG_ERROR("Compression::Compress, compression failed");
            return ERR_UNKNOWN;
        }

        *dst = (char*)pDst;
        dst_len = static_cast<uint32_t>(uDstLen);
        return A_Success;
    }

    AResult Compression::Decompress(char* dst, uint32_t& dst_len, void* src, uint32_t src_len)
    {
        if (!dst || dst_len <= 0 || !src || src_len <= 0)
        {
            LOG_ERROR("Compression::Compress, invalid args");
            return ERR_INVALID_ARG;
        }

        uLong uSrcLen = static_cast<uLong>(src_len);
        const Bytef* pSrc = reinterpret_cast<const Bytef*>(src);
        uLongf* uDstLen = reinterpret_cast<uLongf*>(&dst_len);
        Bytef* pDst = reinterpret_cast<Bytef*>(dst);

        if (Z_OK != uncompress(pDst, uDstLen, pSrc, uSrcLen))
        {
            LOG_ERROR("Compression::Decompress, decompression failed");
            return ERR_UNKNOWN;
        }

        dst_len = static_cast<uint32_t>(*uDstLen);
        return A_Success;
    }

};