export module Aether:D3DDxgiHelper;

import :D3D12Definition;
import :D3DAdapter;
import :Error;

namespace Aether
{
    class DxgiHelper
    {
    public:
        virtual AResult Init(int32_t preferred_adapter, bool debug);
        virtual void Uninit();

        D3DAdapterPtr   ActiveAdapter();
        IDXGIFactory* GetDXGIFactory() { return m_pDxgiFactory.Get(); }
        IDXGIFactory1* GetDXGIFactory1() { return m_pDxgiFactory1.Get(); }
        IDXGIFactory2* GetDXGIFactory2() { return m_pDxgiFactory2.Get(); }
        IDXGIFactory3* GetDXGIFactory3() { return m_pDxgiFactory3.Get(); }
        IDXGIFactory4* GetDXGIFactory4() { return m_pDxgiFactory4.Get(); }
        IDXGIFactory5* GetDXGIFactory5() { return m_pDxgiFactory5.Get(); }
        IDXGIFactory6* GetDXGIFactory6() { return m_pDxgiFactory6.Get(); }
        IDXGIFactory7* GetDXGIFactory7() { return m_pDxgiFactory7.Get(); }
        uint8_t         GetDxgiSubVerion() { return m_iDxgiSubVer; }

    protected:
        IDXGIFactoryPtr     m_pDxgiFactory = nullptr;
        IDXGIFactory1Ptr    m_pDxgiFactory1 = nullptr;
        IDXGIFactory2Ptr    m_pDxgiFactory2 = nullptr;
        IDXGIFactory3Ptr    m_pDxgiFactory3 = nullptr;
        IDXGIFactory4Ptr    m_pDxgiFactory4 = nullptr;
        IDXGIFactory5Ptr    m_pDxgiFactory5 = nullptr;
        IDXGIFactory6Ptr    m_pDxgiFactory6 = nullptr;
        IDXGIFactory7Ptr    m_pDxgiFactory7 = nullptr;

        std::vector<D3DAdapterPtr> m_vAdapterList;
        uint32_t m_iCurAdapterNo = INVALID_ADAPTER_INDEX;
        uint8_t m_iDxgiSubVer = 0;
    };
    using DxgiHelperPtr = std::shared_ptr<DxgiHelper>;
};

