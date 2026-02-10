export module Aether:D3D12RenderView;
import :Engine;
import :EngineDefinition;
import :RHIRenderView;
import :RHIResource;
import :D3D12Resource;
import :D3D12GpuDescriptorAllocator;
import std;


export namespace Aether
{
    /******************************************************************************
    * Render View Handle
    *******************************************************************************/
    class D3D12Rtv
    {
    public:
        D3D12Rtv(AetherEngine* engine, D3D12Resource* res, D3D12_RENDER_TARGET_VIEW_DESC const& rtv_desc);
        ~D3D12Rtv();
        D3D12_CPU_DESCRIPTOR_HANDLE Handle() const { return m_Desc.CpuHandle(); }

    private:
        AetherEngine* m_pEngine = nullptr;
        D3D12Resource* m_pResource = nullptr;
        D3D12GpuDescriptorBlock m_Desc;
    };
    using D3D12RtvPtr = std::shared_ptr<D3D12Rtv>;


    class D3D12Dsv
    {
    public:
        D3D12Dsv(AetherEngine* engine, D3D12Resource* res, D3D12_DEPTH_STENCIL_VIEW_DESC const& dsv_desc);
        ~D3D12Dsv();
        D3D12_CPU_DESCRIPTOR_HANDLE Handle() const { return m_Desc.CpuHandle(); }

    private:
        AetherEngine* m_pEngine = nullptr;
        D3D12Resource* m_pResource = nullptr;
        D3D12GpuDescriptorBlock m_Desc;
    };
    using D3D12DsvPtr = std::shared_ptr<D3D12Dsv>;


    class D3D12Srv
    {
    public:
        D3D12Srv(AetherEngine* engine, D3D12Resource* res, D3D12_SHADER_RESOURCE_VIEW_DESC const& dsv_desc);
        ~D3D12Srv();
        D3D12_CPU_DESCRIPTOR_HANDLE Handle() const { return m_Desc.CpuHandle(); }

    private:
        AetherEngine* m_pEngine = nullptr;
        D3D12Resource* m_pResource = nullptr;
        D3D12GpuDescriptorBlock m_Desc;
    };
    using D3D12SrvPtr = std::shared_ptr<D3D12Srv>;


    class D3D12Uav
    {
    public:
        D3D12Uav(AetherEngine* engine, D3D12Resource* res, D3D12_UNORDERED_ACCESS_VIEW_DESC const& uav_desc);
        ~D3D12Uav();
        D3D12_CPU_DESCRIPTOR_HANDLE Handle() const { return m_Desc.CpuHandle(); }

    private:
        AetherEngine* m_pEngine = nullptr;
        D3D12Resource* m_pResource = nullptr;
        D3D12GpuDescriptorBlock m_Desc;

        ID3D12ResourcePtr m_pCounter = nullptr;
        uint32_t m_iCounterOffset = 0;

    };
    using D3D12UavPtr = std::shared_ptr<D3D12Uav>;

    /******************************************************************************
    * D3D12 Rtv
    *******************************************************************************/
    class D3D12RenderTargetView : public RHIRenderTargetView
    {
    public:
        D3D12RenderTargetView(AetherEngine* engine, D3D12ResourcePtr const& src, uint32_t first_subres, uint32_t num_subres);
        D3D12ResourcePtr const& GetResource() const { return m_pRtvResource; }
        uint32_t GetFirstSubRes() { return m_iFirstSubres; }
        uint32_t GetNumSubRes() { return m_iNumSubres; }
        void ClearColor(float4 const& color);
        virtual D3D12Rtv* GetD3DRtv() = 0;

    protected:
        AetherEngine* m_pEngine = nullptr;
        mutable D3D12RtvPtr m_pRtvHandle = nullptr;
        D3D12ResourcePtr m_pRtvResource = nullptr;
        uint32_t m_iFirstSubres;
        uint32_t m_iNumSubres;
    };
    using D3D12RenderTargetViewPtr = std::shared_ptr<D3D12RenderTargetView>;

    class D3D12Texture2DCubeRtv final : public D3D12RenderTargetView
    {
    public:
        D3D12Texture2DCubeRtv(AetherEngine* engine, RHITexturePtr const& tex_2d_cube, uint32_t first_array_index, uint32_t array_size, uint32_t mip_level);

        D3D12Rtv* GetD3DRtv() override;
    };
    class D3D12Texture3DRtv final : public D3D12RenderTargetView
    {
    public:
        D3D12Texture3DRtv(AetherEngine* engine, RHITexturePtr const& tex_3d, uint32_t array_index, uint32_t first_slice, uint32_t num_slices, uint32_t mip_level);

        D3D12Rtv* GetD3DRtv() override;
    };
    class D3D12TextureCubeFaceRtv final : public D3D12RenderTargetView
    {
    public:
        D3D12TextureCubeFaceRtv(AetherEngine* engine, RHITexturePtr const& tex_cube, uint32_t array_index, ECubeFaceType face, uint32_t mip_level);

        D3D12Rtv* GetD3DRtv() override;
    };

    /******************************************************************************
    * D3D12 Dsv
    *******************************************************************************/
    class D3D12DepthStencilView : public RHIDepthStencilView
    {
    public:
        D3D12DepthStencilView(AetherEngine* engine, D3D12ResourcePtr const& res, uint32_t first_subres, uint32_t num_subres);

        void ClearDepth(float depth = 1.0);
        void ClearStencil(uint32_t stencil = 0);
        void ClearDepthStencil(float depth, uint32_t stencil);
        D3D12ResourcePtr const& GetResource() const { return m_pDsvResource; }
        uint32_t GetFirstSubRes() { return m_iFirstSubres; }
        uint32_t GetNumSubRes() { return m_iNumSubres; }
        virtual D3D12Dsv* GetD3DDsv() = 0;

    protected:
        AetherEngine* m_pEngine = nullptr;
        mutable D3D12DsvPtr m_pDsvHandle = nullptr;
        D3D12ResourcePtr m_pDsvResource = nullptr;
        uint32_t m_iFirstSubres;
        uint32_t m_iNumSubres;
    };
    class D3D12Texture2DDsv : public D3D12DepthStencilView
    {
    public:
        D3D12Texture2DDsv(AetherEngine* engine, RHITexturePtr const& tex_2d, uint32_t first_array_index = 0, uint32_t array_size = 1, uint32_t mip_level = 0);
        D3D12Dsv* GetD3DDsv() override;
    };
    class D3D12TextureCubeFaceDsv : public D3D12DepthStencilView
    {
    public:
        D3D12TextureCubeFaceDsv(AetherEngine* engine, RHITexturePtr const& tex_cube, uint32_t array_index, ECubeFaceType face, uint32_t mip_level);
        D3D12Dsv* GetD3DDsv() override;
    };


    /******************************************************************************
    * D3D12 Srv
    *******************************************************************************/
    class D3D12ShaderResourceView : public RHIShaderResourceView
    {
    public:
        D3D12ShaderResourceView(AetherEngine* engine)
            :RHIShaderResourceView(), m_pEngine(engine)
        {
        }
        virtual D3D12Srv* GetD3DSrv() = 0;

    protected:
        AetherEngine* m_pEngine = nullptr;
        mutable D3D12SrvPtr m_pSrvHandle;
    };
    using D3D12ShaderResourceViewPtr = std::shared_ptr<D3D12ShaderResourceView>;

    class D3D12TextureSrv : public D3D12ShaderResourceView
    {
    public:
        D3D12TextureSrv(AetherEngine* engine, RHITexturePtr const& texture, uint32_t first_array_index, uint32_t array_size, uint32_t first_level, uint32_t num_levels);
        D3D12Srv* GetD3DSrv() override;
    };

    class D3D12BufferSrv final : public D3D12ShaderResourceView
    {
    public:
        D3D12BufferSrv(AetherEngine* engine, RHIBufferPtr const& gbuffer, PixelFormat format, uint32_t first_elem, uint32_t num_elems);
        D3D12Srv* GetD3DSrv() override;
    };


    /******************************************************************************
    * D3D12 Uav
    *******************************************************************************/
    class D3D12UnorderedAccessView : public RHIUnorderedAccessView
    {
    public:
        D3D12UnorderedAccessView(AetherEngine* engine, D3D12ResourcePtr const& src, uint32_t first_subres, uint32_t num_subres);
        virtual D3D12Uav* GetD3DUav() = 0;

        void Clear(float4 const& v) override;
        void Clear(uint4 const& v) override;

        D3D12ResourcePtr const& GetResource() const { return m_pUavResource; }
        uint32_t GetFirstSubRes() { return m_iFirstSubres; }
        uint32_t GetNumSubRes() { return m_iNumSubres; }

    protected:
        AetherEngine* m_pEngine = nullptr;

        mutable D3D12UavPtr m_pUavHandle;
        D3D12ResourcePtr m_pUavResource;
        uint32_t m_iFirstSubres;
        uint32_t m_iNumSubres;
    };
    using D3D12UnorderedAccessViewPtr = std::shared_ptr<D3D12UnorderedAccessView>;


    class D3D12BufferUav final : public D3D12UnorderedAccessView
    {
    public:
        D3D12BufferUav(AetherEngine* engine, RHIBufferPtr const& buf, PixelFormat format, uint32_t first_elem, uint32_t num_elems);
        D3D12Uav* GetD3DUav() override;
    };

};
