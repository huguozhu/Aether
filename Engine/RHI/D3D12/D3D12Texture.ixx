export module Aether:D3D12Texture;
import :RHITexture;
import :Engine;
import :EngineDefinition;
import :D3D12Definition;
import :D3D12RenderView;
import :D3D12Resource;
import :D3D12GpuMemoryAllocator;
import :Log;

export namespace Aether
{
    /******************************************************************************
    * D3D12Texture
    *******************************************************************************/
    class D3D12Texture : public RHITexture, public D3D12Resource
    {
    public:
        D3D12Texture(AetherEngine* engine, const RHITexture::Desc& tex_desc);
        ~D3D12Texture() {};

        virtual AResult Resolve() { return A_Success; }
        DXGI_FORMAT GetD3DFormat() { return m_eDxgiFormat; }

        D3D12SrvPtr const& GetD3DSrv();
        D3D12SrvPtr const& GetD3DSrv(uint32_t first_array_index, uint32_t array_size, uint32_t first_level, uint32_t num_levels);
        D3D12SrvPtr const& GetD3DSrv(uint32_t array_index, ECubeFaceType face, uint32_t first_level, uint32_t num_levels);

        D3D12RtvPtr const& GetD3DRtv();
        D3D12RtvPtr const& GetD3DRtv(uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        D3D12RtvPtr const& GetD3DRtv(uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);
        D3D12RtvPtr const& GetD3DRtv(uint32_t array_index, ECubeFaceType face, uint32_t mip_level);

        D3D12DsvPtr const& GetD3DDsv();
        D3D12DsvPtr const& GetD3DDsv(uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        D3D12DsvPtr const& GetD3DDsv(uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);
        D3D12DsvPtr const& GetD3DDsv(uint32_t array_index, ECubeFaceType face, uint32_t mip_level);

        D3D12UavPtr const& GetD3DUav();
        D3D12UavPtr const& GetD3DUav(uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        D3D12UavPtr const& GetD3DUav(uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);
        D3D12UavPtr const& GetD3DUav(uint32_t first_array_index, uint32_t array_size, ECubeFaceType first_face, uint32_t num_faces, uint32_t mip_level);

        virtual void FillSrvDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t first_level, uint32_t num_levels) { LOG_ERROR("Can't be called."); }
        virtual void FillSrvDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, uint32_t array_index, ECubeFaceType face, uint32_t first_level, uint32_t num_levels) { LOG_ERROR("Can't be called."); }

        virtual void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level) { LOG_ERROR("Can't be called."); }
        virtual void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level) { LOG_ERROR("Can't be called."); }
        virtual void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t array_index, ECubeFaceType face, uint32_t mip_level) { LOG_ERROR("Can't be called."); }

        virtual void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level) { LOG_ERROR("Can't be called."); }
        virtual void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level) { LOG_ERROR("Can't be called."); }
        virtual void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t array_index, ECubeFaceType face, uint32_t mip_level) { LOG_ERROR("Can't be called."); }

        virtual void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level) { LOG_ERROR("Can't be called."); }
        virtual void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level) { LOG_ERROR("Can't be called."); }
        virtual void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, ECubeFaceType first_face, uint32_t num_faces, uint32_t mip_level) { LOG_ERROR("Can't be called."); }

        virtual AResult DumpSubResource2D(BitmapBufferPtr bitmap_data, uint32_t array_index = 0, uint32_t mip_level = 0, Rect<uint32_t>* rect = nullptr) { LOG_ERROR("Can't be called."); return A_Success; }
        virtual AResult DumpSubResource3D(BitmapBufferPtr bitmap_data, uint32_t array_index = 0, uint32_t mip_level = 0, Box<uint32_t>* box = nullptr) { LOG_ERROR("Can't be called."); return A_Success; }
        virtual AResult DumpSubResourceCube(BitmapBufferPtr bitmap_data, ECubeFaceType face, uint32_t array_index = 0, uint32_t mip_level = 0, Rect<uint32_t>* rect = nullptr) { LOG_ERROR("Can't be called."); return A_Success; }

    protected:
        AResult DoCreate(D3D12_RESOURCE_DIMENSION dim, uint32_t width, uint32_t height, uint32_t depth, uint32_t array_size, std::span<BitmapBufferPtr> const& bitmap_datas);

    protected:
        ID3D12ResourcePtr       m_pD3dResource;
        DXGI_FORMAT             m_eDxgiFormat = DXGI_FORMAT_UNKNOWN;
        D3D12GpuMemoryBlock     m_MappedMemoryBlock;        

        uint32_t m_iMappedXOffset;
        uint32_t m_iMappedYOffset;
        uint32_t m_iMappedZOffset;
        uint32_t m_iMappedWidth;
        uint32_t m_iMappedHeight;
        uint32_t m_iMappedDepth;

        std::unordered_map<size_t, D3D12SrvPtr> m_mD3dSrvs;
        std::unordered_map<size_t, D3D12RtvPtr> m_mD3dRtvs;
        std::unordered_map<size_t, D3D12DsvPtr> m_mD3dDsvs;
        std::unordered_map<size_t, D3D12UavPtr> m_mD3dUavs;

    };
    using D3D12TexturePtr = std::shared_ptr<D3D12Texture>;


    /******************************************************************************
    * D3D12Texture2D
    *******************************************************************************/
    class D3D12Texture2D : public D3D12Texture
    {
    public:
        D3D12Texture2D(AetherEngine* engine, const RHITexture::Desc& tex_desc);
        D3D12Texture2D(AetherEngine* engine, ID3D12ResourcePtr const& d3d_tex);

        AResult Create(std::span<BitmapBufferPtr> const& bitmap_datas) override;
        AResult Update(std::span<BitmapBufferPtr> const& bitmap_datas) override;
        AResult Resolve() override;
        AResult DumpSubResource2D(BitmapBufferPtr bitmap_data, uint32_t array_index = 0, uint32_t mip_level = 0, Rect<uint32_t>* rect = nullptr);

    protected:
        void FillSrvDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t first_level, uint32_t num_levels) override;
        void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level) override;
        void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level) override;
        void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level) override;

    private:
        //void FillStageTexture2DDesc(D3D12_TEXTURE2D_DESC& desc);
        //D3D12_TEXTURE2D_DESC m_d3dTexture2DDesc = {};
    };
    using D3D12Texture2DPtr = std::shared_ptr<D3D12Texture2D>;

    /******************************************************************************
    * D3D11TextureCube
    *******************************************************************************/
    class D3D12TextureCube : public D3D12Texture
    {
    public:
        D3D12TextureCube(AetherEngine* engine, const RHITexture::Desc& tex_desc);

        AResult Create(std::span<BitmapBufferPtr> const& bitmap_datas) override;
        AResult Update(std::span<BitmapBufferPtr> const& bitmap_datas) override;
        AResult DumpSubResourceCube(BitmapBufferPtr bitmap_data, ECubeFaceType face, uint32_t array_index = 0, uint32_t mip_level = 0, Rect<uint32_t>* rect = nullptr) override;

    private:
        void FillSrvDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t first_level, uint32_t num_levels);
        void FillSrvDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, uint32_t array_index, ECubeFaceType face, uint32_t first_level, uint32_t num_levels);
        void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t array_index, ECubeFaceType face, uint32_t mip_level);
        void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t array_index, ECubeFaceType face, uint32_t mip_level);
        void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, ECubeFaceType first_face, uint32_t num_faces, uint32_t mip_level);
    };
    using D3D12TextureCubePtr = std::shared_ptr<D3D12TextureCube>;


    /******************************************************************************
    * D3D12Texture3D
    *******************************************************************************/
    class D3D12Texture3D : public D3D12Texture
    {
    public:
        D3D12Texture3D(AetherEngine* engine, const RHITexture::Desc& tex_desc);
        AResult Create(std::span<BitmapBufferPtr> const& bitmap_datas) override;
        AResult Update(std::span<BitmapBufferPtr> const& bitmap_datas) override;
        AResult DumpSubResource3D(BitmapBufferPtr bitmap_data, uint32_t array_index = 0, uint32_t mip_level = 0, Box<uint32_t>* box = nullptr);

    private:
        void FillSrvDesc(D3D12_SHADER_RESOURCE_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t first_level, uint32_t num_levels);
        void FillRtvDesc(D3D12_RENDER_TARGET_VIEW_DESC& desc, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);
        void FillDsvDesc(D3D12_DEPTH_STENCIL_VIEW_DESC& desc, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);
        void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);
        void FillUavDesc(D3D12_UNORDERED_ACCESS_VIEW_DESC& desc, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);
    };
    using D3D12Texture3DPtr = std::shared_ptr<D3D12Texture3D>;

};
