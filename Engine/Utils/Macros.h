

namespace Aether
{
#define AETHER_CHECKFAILED(ret)    ( (ret) != A_Success )

#define AETHER_RETIF_NULL(ptr) { \
    if (!ptr) { \
        LOG_ERROR("ERROR Ptr in null"); \
        return ERR_INVALID_ARG; \
    } \
}
#define AETHER_RETIF_FAIL(func) { \
    AResult __ret = func; \
    if (AETHER_CHECKFAILED(__ret)) { \
        char buf[1024] = {0}; \
        LOG_ERROR("ERROR %s", buf); \
        return __ret; \
    } \
}
#define AETHER_RET_NULL_IF_FAIL(func) { \
    AResult __ret = func; \
    if (AETHER_CHECKFAILED(__ret)) { \
        char buf[1024] = {0}; \
        LOG_ERROR("ERROR %s", buf); \
        return nullptr; \
    } \
}
};