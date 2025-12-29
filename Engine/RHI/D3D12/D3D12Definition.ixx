export module Aether:D3D12Definition;

import <windows.h>;
import <dxgi.h>;
import <dxgi1_2.h>;
import <dxgi1_3.h>;
import <dxgi1_4.h>;
import <dxgi1_5.h>;
import <dxgi1_6.h>;
import <dxgiformat.h>;
import <dxgidebug.h>;
import <guiddef.h>;
import <wrl/client.h>;
import <d3dcompiler.h>;
import <d3d12.h>;
import <d3d12shader.h>;

namespace Aether
{
    using IDXGIFactoryPtr               = Microsoft::WRL::ComPtr<IDXGIFactory>;
    using IDXGIFactory1Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory1>;
    using IDXGIFactory2Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory2>;
    using IDXGIFactory3Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory3>;
    using IDXGIFactory4Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory4>;
    using IDXGIFactory5Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory5>;
    using IDXGIFactory6Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory6>;
    using IDXGIFactory7Ptr              = Microsoft::WRL::ComPtr<IDXGIFactory7>;

    using IDXGIAdapter1Ptr              = Microsoft::WRL::ComPtr<IDXGIAdapter1>;
    using IDXGIAdapter2Ptr              = Microsoft::WRL::ComPtr<IDXGIAdapter2>;
    using IDXGIAdapter3Ptr              = Microsoft::WRL::ComPtr<IDXGIAdapter3>;
    using IDXGIAdapter4Ptr              = Microsoft::WRL::ComPtr<IDXGIAdapter4>;
    using IDXGISwapChain3Ptr            = Microsoft::WRL::ComPtr<IDXGISwapChain3>;

    using ID3D12DebugPtr                = Microsoft::WRL::ComPtr<ID3D12Debug>;
    using ID3D12DevicePtr               = Microsoft::WRL::ComPtr<ID3D12Device>;
    using ID3D12Device10Ptr             = Microsoft::WRL::ComPtr<ID3D12Device10>;
    using ID3D12CommandQueuePtr         = Microsoft::WRL::ComPtr<ID3D12CommandQueue>;
    using ID3D12CommandSignaturePtr     = Microsoft::WRL::ComPtr<ID3D12CommandSignature>;
    using ID3D12CommandAllocatorPtr     = Microsoft::WRL::ComPtr<ID3D12CommandAllocator>;
    using ID3D12GraphicsCommandListPtr  = Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>;
    using ID3D12RootSignaturePtr        = Microsoft::WRL::ComPtr<ID3D12RootSignature>;

    using ID3D12FencePtr                = Microsoft::WRL::ComPtr<ID3D12Fence>;
    using ID3D12DescriptorHeapPtr       = Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>;
    using ID3D12ResourcePtr             = Microsoft::WRL::ComPtr<ID3D12Resource>;
    using ID3D12PipelineStatePtr        = Microsoft::WRL::ComPtr<ID3D12PipelineState>;
    using ID3DBlobPtr                   = Microsoft::WRL::ComPtr<ID3DBlob>;
    using ID3D12StateObjectPtr          = Microsoft::WRL::ComPtr<ID3D12StateObject>;

    export inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            // 抛出异常或处理错误
            //throw std::runtime_error("DirectX operation failed");
        }
    }

    

};
