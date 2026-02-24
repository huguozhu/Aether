module;
#include <windows.h>

export module Aether:D3D12Window;
import :Rect;
import :EngineDefinition;
import :D3D12Definition;
import :D3D12FrameBuffer;
import :RHIContext;

export namespace Aether
{
	class AetherEngine;
	class D3DAdapter;
	class D3D12Window : public D3D12FrameBuffer
	{
	public:
		D3D12Window(AetherEngine* engine);
		~D3D12Window();

		AResult Create(D3DAdapter* adapter, std::string const name, void* native_wnd);

		AResult SwapBuffers();

	private:
		std::string			m_szName;
		HWND				m_hWnd = nullptr;
		D3DAdapter*			m_pAdapter = nullptr;

		IDXGISwapChain3Ptr	m_pSwapChain = nullptr;
		Rect<int32_t>		m_Rect;
		uint32_t			m_iCurBackBufferIndex = 0;

		std::array<RHITexturePtr, RHIContext::NUM_BACK_BUFFERS> m_vBackBufferRtvTexes;
		std::array<RHIRenderTargetViewPtr, RHIContext::NUM_BACK_BUFFERS> m_vBackBufferRtvs;

		RHITexturePtr m_vBackBufferDsvTex;
		RHIDepthStencilViewPtr m_vBackBufferDsv;
	};
	using D3D12WindowPtr = std::shared_ptr<D3D12Window>;
};