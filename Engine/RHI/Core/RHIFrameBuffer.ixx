export module Aether:RHIFrameBuffer;
import :RHITexture;
import :RHIViewport;
import :Engine;
import :EngineDefinition;
import :Vector;

export namespace Aether
{
    constexpr uint8_t MAX_COLOR_ATTACHMENTS = 8;
    class D3D12CommandList;
    class RHIFrameBuffer
    {
    public:
        enum Attachment : uint8_t
        {
            Color0 = 0,
            Color1,
            Color2,
            Color3,
            Color4,
            Color5,
            Color6,
            Color7,
            DepthStencil,
        };
        using RtvArray = std::array<RHIRenderTargetViewPtr, MAX_COLOR_ATTACHMENTS>;
        enum class LoadAction
        {
            Load = 0,
            DontCare,
            Clear,
        };
        enum class StoreAction
        {
            Store = 0,
            DontCare,
        };
        struct LoadOption
        {
            LoadOption()
                : loadAction(LoadAction::DontCare)
                , clearColor({ 1.0, 1.0, 1.0, 1.0 })
            {
            }

            LoadOption(LoadAction _loadAction)
                : loadAction(_loadAction)
                , clearColor({ 1.0, 1.0, 1.0, 1.0 })
            {
            }

            LoadOption(float4 colorClear)
                : loadAction(LoadAction::Clear)
                , clearColor(colorClear)
            {
            }

            LoadOption(float depthClear)
                : loadAction(LoadAction::Clear)
                , clearDepth(depthClear)
            {
            }

            LoadOption(int32_t stencilClear)
                : loadAction(LoadAction::Clear)
                , clearStencil(stencilClear)
            {
            }

            LoadOption(const LoadOption& rhs)
                : loadAction(rhs.loadAction)
                , clearColor(rhs.clearColor)
            {
            }

            LoadOption& operator=(const LoadOption& rhs)
            {
                loadAction = rhs.loadAction;
                clearColor = rhs.clearColor;
                return *this;
            }

            LoadAction loadAction;
            union {
                float4 clearColor;
                float clearDepth;
                int32_t clearStencil;
            }; // clear value used when loadAction is LoadAction::Clear
        };

        struct StoreOption
        {
            StoreOption()
                : storeAction(StoreAction::Store)
            {
            }
            StoreOption(StoreAction _storeAction)
                : storeAction(_storeAction)
            {
            }
            StoreAction storeAction = StoreAction::Store;
        };

        bool IsDirty() const  { return m_bDirty; }
        void SetDirty(bool v) { m_bDirty = v; }

        void SetViewport(RHIViewport viewport) { m_Viewport = viewport; }
        RHIViewport const& GetViewport() { return m_Viewport;}

        void AttachTargetView(Attachment att, RHIRenderTargetViewPtr const& view);
        void DetachTargetView(Attachment att);
        void DetachAllTargetView();

        void AttachDepthStencilView(RHIDepthStencilViewPtr const& view) { m_pDepthStencilView = view; }
        void DetachDepthStencilView() { m_pDepthStencilView = nullptr; }

        RHIRenderTargetViewPtr      GetRenderTarget(Attachment att) const;
        RtvArray const& GetRenderTargets() const { return m_vRenderTargets; }
        RHIDepthStencilViewPtr const& GetDepthStencilView() const { return m_pDepthStencilView; }

        size_t Hash() { return 0; }
                
    protected:
        RHIFrameBuffer() = default;
        ~RHIFrameBuffer() = default;

        bool                    m_bDirty = false;
        RHIViewport             m_Viewport;
        RtvArray                m_vRenderTargets;
        RHIDepthStencilViewPtr  m_pDepthStencilView;

        LoadOption m_colorLoadOptions[MAX_COLOR_ATTACHMENTS];
        StoreOption m_colorStoreOptions[MAX_COLOR_ATTACHMENTS];

        LoadOption m_depthLoadOption;
        StoreOption m_depthStoreOption;
    };

};
