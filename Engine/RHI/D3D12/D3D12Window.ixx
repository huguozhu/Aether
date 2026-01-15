module;
#include <windows.h>

export module Aether:D3D12Window;
import :Rect;
import :EngineDefinition;
import :D3D12Definition;

export namespace Aether
{
	class AetherEngine;
	class D3DAdapter;
	class D3D12Window
	{
	public:
		D3D12Window(AetherEngine* engine);
		~D3D12Window();

		AResult Create(D3DAdapter* adapter, std::string const name, void* native_wnd);

		AResult SwapBuffers();

	private:
		AetherEngine*		m_pEngine = nullptr;
		std::string			m_szName;
		HWND				m_hWnd = nullptr;
		D3DAdapter*			m_pAdapter = nullptr;

		IDXGISwapChain3Ptr	m_pSwapChain = nullptr;
		Rect<int32_t>		m_Rect;
		uint32_t			m_iCurBackBufferIndex = 0;

	};
	using D3D12WindowPtr = std::shared_ptr<D3D12Window>;
};