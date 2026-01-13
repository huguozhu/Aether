module;
#include <span>

export module Aether:RHITexture;
import :RHIResource;
import :Engine;
import :EngineDefinition;
import :Rect;
import :Box;
import :Utils;
import :MathUtils;
import :Format;
import :Buffer;

export namespace Aether
{
    enum class ETextureType
    {
        None,
        Tex2D,
        Tex3D,
        Cube,
    };

    using ResourceFlags = uint64_t;
    constexpr uint64_t RESOURCE_FLAG_NONE                  = UINT64_C(0);

    constexpr uint64_t RESOURCE_FLAG_CPU_READ              = UINT64_C(0x0000000000000001);        // Copy data from gpu-buffer --> cpu-buffer
    constexpr uint64_t RESOURCE_FLAG_CPU_WRITE             = UINT64_C(0x0000000000000002);        // Copy data from cpu-buffer --> gpu-buffer
    constexpr uint64_t RESOURCE_FLAG_GPU_READ              = UINT64_C(0x0000000000000004);        // Shader Resource View
    constexpr uint64_t RESOURCE_FLAG_GPU_WRITE             = UINT64_C(0x0000000000000008);        // RenderTarget or DepthStencil View

    constexpr uint64_t RESOURCE_FLAG_UAV                   = UINT64_C(0x0000000000000100);        // Unordered Access View
    constexpr uint64_t RESOURCE_FLAG_GENERATE_MIPS         = UINT64_C(0x0000000000000200);        // Generate Mips
    constexpr uint64_t RESOURCE_FLAG_GPU_STRUCTURED        = UINT64_C(0x0000000000000400);        // HLSL: StructuredBuffer<xxx>
    constexpr uint64_t RESOURCE_FLAG_DRAW_INDIRECT_ARGS    = UINT64_C(0x0000000000001000);        // Used in Indirect Draw's
    constexpr uint64_t RESOURCE_FLAG_RAW                   = UINT64_C(0x0000000000002000);        // HLSL: RWByteAddressBuffer
    constexpr uint64_t RESOURCE_FLAG_APPEND                = UINT64_C(0x0000000000004000);        // HLSL: AppendStructuredBuffer<xxx> 
    constexpr uint64_t RESOURCE_FLAG_COUNTER               = UINT64_C(0x0000000000008000);        // HLSL: ConsumeStructuredBuffer<xxx> 
   
    class RHITexture : public RHIResource
    {
    public:
        struct Desc
        {
            ETextureType type = ETextureType::None;
            uint32_t width = 0;
            uint32_t height = 0;
            uint32_t depth = 1;
            uint32_t num_mips = 1;          // if = 0 --> create max mipmap auto
            uint32_t num_samples = 1;
            uint32_t num_array = 1;
            PixelFormat format = PixelFormat::Unknown;
            ResourceFlags flags = RESOURCE_FLAG_NONE;
        };

        Desc const&         Descriptor()    const { return m_desc; };
        ETextureType        Type()          const { return m_desc.type; }
        uint32_t            Width(uint32_t mip_level = 0)   const { AETHER_ASSERT(mip_level < m_desc.num_mips); return std::max(1U, m_desc.width >> mip_level); }
        uint32_t            Height(uint32_t mip_level = 0)  const { AETHER_ASSERT(mip_level < m_desc.num_mips); return std::max(1U, m_desc.height >> mip_level); }
        uint32_t            Depth(uint32_t mip_level = 0)   const { AETHER_ASSERT(mip_level < m_desc.num_mips); return std::max(1U, m_desc.depth >> mip_level); }
        uint32_t            NumMips()       const { return m_desc.num_mips; }
        uint32_t            NumSamples()    const { return m_desc.num_samples; }
        uint32_t            NumArray()      const { return m_desc.num_array; }
        PixelFormat         Format()        const { return m_desc.format; }
        ResourceFlags       Flags()         const { return m_desc.flags; }

        virtual AResult     Create(std::span<BitmapBufferPtr> const& bitmap_datas) = 0;
        virtual AResult     Update(std::span<BitmapBufferPtr> const& bitmap_datas) = 0;
        virtual AResult     GenerateMipMap() { return A_Success; }

        virtual AResult DumpSubResource2D(BitmapBufferPtr bitmap_data, uint32_t array_index = 0, uint32_t mip_level = 0, Rect<uint32_t>* rect = nullptr) = 0;
        virtual AResult DumpSubResource3D(BitmapBufferPtr bitmap_data, uint32_t array_index = 0, uint32_t mip_level = 0, Box<uint32_t>* box = nullptr) = 0;
        virtual AResult DumpSubResourceCube(BitmapBufferPtr bitmap_data, ECubeFaceType face, uint32_t array_index = 0, uint32_t mip_level = 0, Rect<uint32_t>* rect = nullptr) = 0;

        AResult DumpToFile(std::string path, BitmapBufferPtr bitmap_data)
        {
            if (!bitmap_data)
                return -1;
            bitmap_data->DumpToFile(path);
            return A_Success;
        }

        AResult DumpToFile(std::string path, ECubeFaceType face = ECubeFaceType::Num)
        {
            BitmapBufferPtr bitmap_data = MakeSharedPtr<BitmapBuffer>();
            switch (m_desc.type)
            {
            case ETextureType::Tex2D:
            {
                AETHER_RETIF_FAIL(this->DumpSubResource2D(bitmap_data));
                break;
            }
            case ETextureType::Tex3D:
            {
                AETHER_RETIF_FAIL(this->DumpSubResource3D(bitmap_data));
                break;
            }
            case ETextureType::Cube:
            {
                AETHER_RETIF_FAIL(this->DumpSubResourceCube(bitmap_data, face));
                break;
            }
            }

            bitmap_data->DumpToFile(path);
            return A_Success;
        }

    protected:
        RHITexture(AetherEngine* engine, const Desc& desc)
            : m_pEngine(engine), m_desc(desc)
        {
        }
        virtual ~RHITexture() = default;

        AetherEngine* m_pEngine = nullptr;
        Desc            m_desc;
    };

};