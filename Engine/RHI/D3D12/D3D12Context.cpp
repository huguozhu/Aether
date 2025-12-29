module;
#include <windows.h>

module Aether:D3D12Context;
import :D3D12Context;
import :D3D12Definition;
import :dll_loader;
import :log;

const char* GetD3D12FeatureLevelStr(D3D_FEATURE_LEVEL feature_level)
{
    switch (feature_level)
    {
    case D3D_FEATURE_LEVEL_12_2:
        return "12.2";
    case D3D_FEATURE_LEVEL_12_1:
        return "12.1";
    case D3D_FEATURE_LEVEL_12_0:
        return "12.0";
    default:
        return "unknown";
    }
}

namespace Aether
{
    static DllLoader s_d3d12("d3d12.dll");
    static decltype(&::D3D12GetDebugInterface) FUNC_D3D12GetDebugInterface = nullptr;
    static decltype(&::D3D12CreateDevice) FUNC_D3D12CreateDevice = nullptr;

    D3D12Context::D3D12Context(AetherEngine* engine)
        :RHIContext(engine)
    {
        this->Init();
    }

    AResult D3D12Context::Init()
    {
        AETHER_RETIF_FAIL(DxgiHelper::Init(m_pEngine->GetPreferredAdapter(), m_pEngine->EnableDebug()));
        do {
            if (!s_d3d12.Load())
            {
                LOG_ERROR("load %s fail", s_d3d12.dllname.c_str());
                return ERR_NOT_SUPPORT;
            }

            if (!FUNC_D3D12CreateDevice)
            {
                FUNC_D3D12CreateDevice = (decltype(FUNC_D3D12CreateDevice))s_d3d12.FindSymbol("D3D12CreateDevice");
                if (!FUNC_D3D12CreateDevice)
                {
                    LOG_ERROR("Function D3D12CreateDevice not found.");
                    return ERR_NOT_SUPPORT;
                }
            }

            D3D_FEATURE_LEVEL feature_levels[] =
            {
                D3D_FEATURE_LEVEL_12_2,
                D3D_FEATURE_LEVEL_12_1,
                D3D_FEATURE_LEVEL_12_0
            };
            uint32_t feature_level_start_index = 0;
            HRESULT hr = S_OK;
            uint32_t feature_level_count = sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL);
            for (; feature_level_start_index < feature_level_count; feature_level_start_index++)
            {
                hr = FUNC_D3D12CreateDevice(m_vAdapterList[m_iCurAdapterNo]->DXGIAdapter(),
                    feature_levels[feature_level_start_index], __uuidof(ID3D12Device),
                    (void**)m_pDevice.GetAddressOf());
                if (SUCCEEDED(hr))
                    break;
            }
            if (FAILED(hr))
            {
                LOG_ERROR("D3D12CreateDevice Error, hr:%x", hr);
                break;
            }
            LOG_INFO("device supported feature level %s", GetD3D12FeatureLevelStr(feature_levels[feature_level_start_index]));

        } while (1);

        return A_Success;
    }


    AResult D3D12Context::CheckCapabilitySetSupport()
    {
        return A_Success;
    }

};