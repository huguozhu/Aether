module;
#include <windows.h>
#include <dxgi.h>
#include <dxgi1_3.h>
#include <d3d12.h>

module Aether:D3D12Window;
import :D3D12Window;
import :D3D12Context;
import :D3D12Definition;
import :EngineDefinition;
import :RHIContext;
import :D3D12Definition;
import :D3D12Texture;
import :Error;

namespace Aether
{
	D3D12Window::D3D12Window(AetherEngine* engine)
		:D3D12FrameBuffer(engine)
	{
	}
	D3D12Window::~D3D12Window()
	{
		m_pAdapter = nullptr;
		m_szName.clear();
		m_hWnd = nullptr;
		Sleep(100);
		m_pSwapChain.Reset();
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

			DXGI_SWAP_CHAIN_DESC1 sc_desc = {};
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

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC sfc_desc = {};
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

			for (uint32_t i = 0; i < RHIContext::NUM_BACK_BUFFERS; i++)
			{
				ID3D12ResourcePtr v = nullptr;
				ThrowIfFailed(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(v.GetAddressOf())));
				m_vBackBufferRtvTexes[i] = d3d12_rc.CreateTexture2D(v);
				m_vBackBufferRtvs[i] = d3d12_rc.Create2DRenderTargetView(m_vBackBufferRtvTexes[i], 0, 1, 0);
			}
			this->AttachTargetView(Attachment::Color0, m_vBackBufferRtvs[0]);

			RHITexture::Desc desc = {};
			desc.width = m_Rect.width;
			desc.height = m_Rect.height;
			desc.type = ETextureType::Tex2D;
			desc.format = PixelFormat::D24S8;
			desc.flags = RESOURCE_FLAG_GPU_READ | RESOURCE_FLAG_GPU_WRITE;
			m_vBackBufferDsvTex = d3d12_rc.CreateTexture2D(desc);

			for (uint32_t i = 0; i < RHIContext::NUM_BACK_BUFFERS; i++)
			{
				m_vBackBufferDsvs[i] = d3d12_rc.Create2DDepthStencilView(m_vBackBufferDsvTex);
			}
			this->AttachDepthStencilView(m_vBackBufferDsvs[0]);

			m_iCurBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();
		} while (0);
		return A_Success;
	}

	AResult D3D12Window::SwapBuffers()
	{
		if (m_pSwapChain)
		{
			D3D12Context& rc = static_cast<D3D12Context&>(m_pEngine->RHIContextInstance());
			ID3D12Device* pDevice = rc.GetD3D12Device();			
			ID3D12GraphicsCommandList* cmd_list = rc.D3DRenderCmdList();
			D3D12Texture* rt_tex = (D3D12Texture*)m_vBackBufferRtvTexes[m_iCurBackBufferIndex].get();
			rt_tex->UpdateResourceBarrier(cmd_list, 0, D3D12_RESOURCE_STATE_PRESENT);
			rc.CommitRenderCmd();
			HRESULT hr = m_pSwapChain->Present(0, 0);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12Window::SwapBuffers() error;");
				return ERR_SYSTEM_ERROR;
			}

			m_iCurBackBufferIndex = m_pSwapChain->GetCurrentBackBufferIndex();
			this->AttachTargetView(Attachment::Color0, m_vBackBufferRtvs[m_iCurBackBufferIndex]);			
		}
		return A_Success;
	}
};