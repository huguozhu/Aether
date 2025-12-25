module;
#include <SDKDDKVer.h>
#include <windows.h>
#include <Shlwapi.h>
#include <wincodec.h>
#include "imgui_impl_win32.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_vulkan.h"
#include "aether.config.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

module Aether.SampleCommon:AppFramework;
import :AppFramework;
import :AppImgui;
import Aether.Utils;
import Aether.Kernel;

namespace Aether
{
    AResult AppFramework::InitContext(void* device, void* native_wnd)
    {
        m_pEngine = MakeSharedPtr<AetherEngine>();
        EngineInitInfo info{ ERHIType::D3D12 };
        m_pEngine->StartEngine(info);
        AETHER_RETIF_FAIL(m_pEngine->Initialize(device, native_wnd));

        return A_Success;
    }
    void AppFramework::IMGUI_Begin()
    {
        if (m_pEngine->GetRHIType() == ERHIType::D3D12)
            ImGui_ImplDX12_NewFrame();
        else if (m_pEngine->GetRHIType() == ERHIType::Vulkan)
            ImGui_ImplVulkan_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();
    }

    void AppFramework::IMGUI_Rendering()
    {
        //D3D11Context* rc_d3d = static_cast<D3D11Context*>(&m_pContext->RHIContextInstance());
        //D3D11FrameBuffer* fb = static_cast<D3D11FrameBuffer*>(rc_d3d->GetFinalRHIFrameBuffer().get());
        //ID3D11RenderTargetView* view = fb->GetRenderTargetView();

        //ImGui::Render();
        //rc_d3d->GetD3D11DeviceContext()->OMSetRenderTargets(1, &view, NULL);
        //ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }
    AResult AppFramework::RenderFrame()
    {
        return A_Success;
    }

    IWICImagingFactory* g_pIWICFactory;
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
            return true;
        switch (uMsg)
        {
        case WM_ACTIVATE:
            break;

        case WM_PAINT:
            break;

        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    HWND InitWindow(std::string const& name, uint32_t width, uint32_t height)
    {
        uint32_t w = width;
        uint32_t h = height;
        HINSTANCE hInst = GetModuleHandle(nullptr);

        WNDCLASSEXA wc;
        ZeroMemory(&wc, sizeof(wc));
        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = WndProc;
        wc.hIcon = nullptr;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hInstance = hInst;
        wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = name.c_str();
        RegisterClassExA(&wc);

        uint32_t style = WS_OVERLAPPEDWINDOW;

        RECT rc = { 0, 0, (LONG)w, (LONG)h };
        AdjustWindowRect(&rc, style, false);
        HWND wnd = CreateWindowA(name.c_str(), name.c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT,
            w, h, nullptr, nullptr, hInst, nullptr);

        ShowWindow(wnd, SW_SHOWNORMAL);
        UpdateWindow(wnd);
        return wnd;
    }

    AppFramework::AppFramework(std::string const& name)
        :m_szName(name)
    {
        IMGUI_Init();
    }

    AResult AppFramework::Run()
    {
        CoInitializeEx(NULL, COINIT_MULTITHREADED);
        CoCreateInstance(CLSID_WICImagingFactory1, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&g_pIWICFactory));

        HWND wnd = InitWindow(m_szName, DEFAULT_WND_WIDTH, DEFAULT_WND_HEIGHT);
        this->InitContext(NULL, (void*)wnd);

        if (!m_bInit)
        {
            AETHER_RETIF_FAIL(this->OnCreate());
            m_bInit = true;
        }

        ImGui_ImplWin32_Init(wnd);        
        //{
        //    D3D12Context* rc_d3d = static_cast<D3D12Context*>(&m_pContext->RHIContextInstance());
        //    ImGui_ImplDX12_Init(rc_d3d->GetD3D12Device(), )
        //}

        bool get_msg = false;
        MSG  msg;
        ::PeekMessage(&msg, nullptr, 0, 0, PM_NOREMOVE);
        while (WM_QUIT != msg.message)
        {
            get_msg = (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) != 0);

            if (get_msg)
            {
                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);
            }
            else
            {
                AETHER_RETIF_FAIL(this->OnUpdate());
            }
        }

        this->OnDestroy();
        DestroyWindow(wnd);
        CoUninitialize();
        return A_Success;
    }

    std::string AppFramework::FullPath(std::string relativePath)
    {
        return std::string{ AETHER_SAMPLES_DIR }.append("/") + relativePath;
    }
};