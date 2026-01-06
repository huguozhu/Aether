export module Aether:Error;

import :Log;
import :EngineDefinition;
import <string>;

export namespace Aether
{
    /**
    * AResult contains three components:
    * [31, 24] error code
    * [23, 14] file uid
    * [13,  0] line number
    *
    * 31 ----------- 24 23 -------- 14 13 ----------- 0
    * |  error code   |||  file uid  |||  line number |
    * +---------------+++------------+++--------------+
    *
    */
    constexpr uint32_t ERR_ERRORCODE_BITS = 8;       // 0 ~ 255
    constexpr uint32_t ERR_FILEUID_BITS = 10;      // 0 ~ 1023
    constexpr uint32_t ERR_LINENUM_BITS = 14;      // 0 ~ 16383

    constexpr uint32_t ERR_ERRORCODE_SHIFT = (ERR_FILEUID_BITS + ERR_LINENUM_BITS);
    constexpr uint32_t ERR_ERRORCODE_MASK = ((1 << ERR_ERRORCODE_BITS) - 1);
    constexpr uint32_t ERR_FILEUID_SHIFT = (ERR_LINENUM_BITS);
    constexpr uint32_t ERR_FILEUID_MASK = ((1 << ERR_FILEUID_BITS) - 1);
    constexpr uint32_t ERR_LINENUM_MASK = ((1 << ERR_LINENUM_BITS) - 1);
    constexpr uint32_t ERR_LINENUM_MAX = ERR_LINENUM_MASK;

    constexpr uint32_t  ERR_ERRORCODE(uint32_t err) { return (((err) >> ERR_ERRORCODE_SHIFT) & ERR_ERRORCODE_MASK); }
    constexpr uint32_t  ERR_FILEUID(uint32_t err) { return (((err) >> ERR_FILEUID_SHIFT) & ERR_FILEUID_MASK); }
    constexpr uint32_t  ERR_LINENUM(uint32_t err) { return ((err)&ERR_LINENUM_MASK); }
    constexpr bool      ERR_EQUAL(uint32_t err1, uint32_t err2) { return (ERR_ERRORCODE(err1) == ERR_ERRORCODE(err2)); }
    constexpr uint32_t  ERR_GEN(uint32_t errorcode) { return (((errorcode) << ERR_ERRORCODE_SHIFT) | __LINE__); }


    /**
        Use follow ERR MACRO as a RETURN value
    */
    constexpr uint32_t ERR_UNKNOWN = ERR_GEN(AETHER_ERR_UNKNOWN);
    constexpr uint32_t ERR_INVALID_ARG = ERR_GEN(AETHER_ERR_INVALID_ARG);
    constexpr uint32_t ERR_INVALID_INIT = ERR_GEN(AETHER_ERR_INVALID_INIT);
    constexpr uint32_t ERR_INVALID_INVOKE_FLOW = ERR_GEN(AETHER_ERR_INVALID_INVOKE_FLOW);
    constexpr uint32_t ERR_INVALID_SHADER = ERR_GEN(AETHER_ERR_INVALID_SHADER);
    constexpr uint32_t ERR_INVALID_MODEL_FILE = ERR_GEN(AETHER_ERR_INVALID_MODEL_FILE);
    constexpr uint32_t ERR_INVALID_DATA = ERR_GEN(AETHER_ERR_INVALID_DATA);
    constexpr uint32_t ERR_NOT_SUPPORT = ERR_GEN(AETHER_ERR_NOT_SUPPORT);
    constexpr uint32_t ERR_NOT_IMPLEMENTED = ERR_GEN(AETHER_ERR_NOT_IMPLEMENTED);
    constexpr uint32_t ERR_NO_MEM = ERR_GEN(AETHER_ERR_NO_MEM);
    constexpr uint32_t ERR_NO_DATA = ERR_GEN(AETHER_ERR_NO_DATA);
    constexpr uint32_t ERR_FILE_NOT_FOUND = ERR_GEN(AETHER_ERR_FILE_NOT_FOUND);
    constexpr uint32_t ERR_SYSTEM_ERROR = ERR_GEN(AETHER_ERR_SYSTEM_ERROR);

    constexpr bool AETHER_CHECKFAILED(AResult ret) { return (ret != A_Success); }
    constexpr AResult AETHER_RETIF_NULL(void* ptr)
    {
        if (!ptr)
        {
            LOG_ERROR("ERROR Ptr in null");
            return ERR_INVALID_ARG;
        }
        return A_Success;
    }
    AResult AETHER_RETIF_FAIL(AResult ret)
    {
        if (AETHER_CHECKFAILED(ret))
        {
            char buf[1024] = { 0 };
            LOG_ERROR("ERROR %s", buf);
            return ret;
        }
        return A_Success;
    }
    constexpr void* AETHER_RET_NULL_IF_FAIL(AResult ret)
    {
        if (AETHER_CHECKFAILED(ret))
        {
            char buf[1024] = { 0 };
            LOG_ERROR("ERROR %s", buf);
            return nullptr;
        }
        return nullptr;
    }
};