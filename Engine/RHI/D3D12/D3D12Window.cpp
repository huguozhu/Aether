module;
#include <windows.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d12.h>

module Aether:D3D12Window;
import :D3D12Window;
import :D3D12Context;
import :EngineDefinition;
import :RHIContext;
import :D3D12Definition;

namespace Aether
{
	D3D12Window::D3D12Window(AetherEngine* engine)
		:m_pEngine(engine)
	{


	}

	AResult D3D12Window::Create(D3DAdapter* adapter, std::string const name, void* native_wnd)
	{
		m_pAdapter = adapter;
		m_szName = name;
		m_hWnd = (HWND)native_wnd;

		do
		{
			RECT rc;
			GetClientRect(m_hWnd, &rc);
			m_Rect = { rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
			m_Rect = m_Rect;

			DXGI_SWAP_CHAIN_DESC1			sc_desc;
			sc_desc.BufferCount = RHIContext::NUM_BACK_BUFFERS;
			sc_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			sc_desc.Width = m_Rect.width;
			sc_desc.Height = m_Rect.height;
			sc_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			sc_desc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			sc_desc.SampleDesc.Count = 1;
			sc_desc.SampleDesc.Quality = 0;
			sc_desc.Scaling = DXGI_SCALING_STRETCH;
			sc_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			sc_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfc_desc;
			sfc_desc.RefreshRate.Numerator = (UINT)m_pEngine->GetFpsLimitType();
			sfc_desc.RefreshRate.Denominator = 1;
			sfc_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			sfc_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			sfc_desc.Windowed = !m_pEngine->IsFullScreen();

			D3D12Context& d3d12_rc = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance());
			IDXGIFactory4* factory = (IDXGIFactory4*)d3d12_rc.GetDXGIFactory4();
			ID3D12Device* pD3DDevice = d3d12_rc.GetD3D12Device();
			ID3D12CommandQueue* pD3DCommandQueue = d3d12_rc.GetD3D12CommandQueue();
			IDXGISwapChain1Ptr sc;
			HRESULT hr = factory->CreateSwapChainForHwnd(pD3DCommandQueue, m_hWnd, &sc_desc, &sfc_desc, nullptr, sc.GetAddressOf());
			if (FAILED(hr))
			{
				LOG_ERROR("CreateSwapChain Error: %x.", hr);
				break;
			}
			sc.As(&m_pSwapChain);
			m_pSwapChain->SetFullscreenState(m_pEngine->IsFullScreen(), nullptr);
			m_iCurBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();


		} while (0);
		return A_Success;
	}

};