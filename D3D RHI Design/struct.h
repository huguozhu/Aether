// 基础类型定义
enum class API_TYPE {
    D3D12,
    VULKAN,
    METAL,
    OPENGL
};

enum class FORMAT {
    UNKNOWN,
    R8G8B8A8_UNORM,
    R8G8B8A8_SRGB,
    R32G32B32A32_FLOAT,
    D32_FLOAT,
    D24_UNORM_S8_UINT,
    // ... 其他格式
};

enum class RESOURCE_STATE {
    COMMON,
    VERTEX_AND_CONSTANT_BUFFER,
    INDEX_BUFFER,
    RENDER_TARGET,
    DEPTH_WRITE,
    DEPTH_READ,
    PIXEL_SHADER_RESOURCE,
    NON_PIXEL_SHADER_RESOURCE,
    COPY_SRC,
    COPY_DEST,
    PRESENT,
    GENERIC_READ,
    UNORDERED_ACCESS
};

enum class RESOURCE_HEAP_TYPE {
    DEFAULT,
    UPLOAD,
    READBACK
};

enum class SHADER_STAGE {
    VERTEX,
    PIXEL,
    DOMAIN,
    HULL,
    GEOMETRY,
    COMPUTE
};

enum class PRIMITIVE_TOPOLOGY {
    UNDEFINED,
    POINT_LIST,
    LINE_LIST,
    LINE_STRIP,
    TRIANGLE_LIST,
    TRIANGLE_STRIP
};

enum class BLEND_MODE {
    ZERO,
    ONE,
    SRC_COLOR,
    INV_SRC_COLOR,
    SRC_ALPHA,
    INV_SRC_ALPHA,
    DEST_ALPHA,
    INV_DEST_ALPHA,
    DEST_COLOR,
    INV_DEST_COLOR
};

enum class COMPARISON_FUNC {
    NEVER,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS
};

enum class TEXTURE_ADDRESS_MODE {
    WRAP,
    MIRROR,
    CLAMP,
    BORDER
};

enum class FILTER_MODE {
    MIN_MAG_MIP_POINT,
    MIN_MAG_POINT_MIP_LINEAR,
    MIN_POINT_MAG_LINEAR_MIP_POINT,
    MIN_POINT_MAG_MIP_LINEAR,
    MIN_LINEAR_MAG_MIP_POINT,
    MIN_LINEAR_MAG_POINT_MIP_LINEAR,
    MIN_MAG_LINEAR_MIP_POINT,
    MIN_MAG_MIP_LINEAR,
    ANISOTROPIC
};

struct Viewport {
    float x;
    float y;
    float width;
    float height;
    float minDepth;
    float maxDepth;
};

struct Rect {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct ClearValue {
    union {
        float color[4];
        struct {
            float depth;
            uint8_t stencil;
        } depthStencil;
    };
    
    FORMAT format;
};