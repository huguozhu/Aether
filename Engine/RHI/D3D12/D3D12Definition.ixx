export module Aether.RHI:d3d12_definition;

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

export namespace Aether
{

    template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    using IDXGIFactoryPtr = ComPtr<IDXGIFactory>;
    using IDXGIFactory1Ptr = ComPtr<IDXGIFactory1>;
    using IDXGIFactory2Ptr = ComPtr<IDXGIFactory2>;
    using IDXGIFactory3Ptr = ComPtr<IDXGIFactory3>;
    using IDXGIFactory4Ptr = ComPtr<IDXGIFactory4>;
    using IDXGIFactory5Ptr = ComPtr<IDXGIFactory5>;
    using IDXGIFactory6Ptr = ComPtr<IDXGIFactory6>;
    using IDXGIAdapter1Ptr = ComPtr<IDXGIAdapter1>;
    using IDXGIAdapter1Ptr = ComPtr<IDXGIAdapter1>;
    using IDXGISwapChain3Ptr = ComPtr<IDXGISwapChain3>;

    using ID3D12DebugPtr = ComPtr<ID3D12Debug>;
    using ID3D12DevicePtr = ComPtr<ID3D12Device>;
    using ID3D12CommandQueuePtr = ComPtr<ID3D12CommandQueue>;
    using ID3D12CommandSignaturePtr = ComPtr<ID3D12CommandSignature>;
    using ID3D12CommandAllocatorPtr = ComPtr<ID3D12CommandAllocator>;
    using ID3D12GraphicsCommandListPtr = ComPtr<ID3D12GraphicsCommandList>;

    using ID3D12FencePtr = ComPtr<ID3D12Fence>;
    using ID3D12DescriptorHeapPtr = ComPtr<ID3D12DescriptorHeap>;
    using ID3D12ResourcePtr = ComPtr<ID3D12Resource>;
    using ID3D12PipelineStatePtr = ComPtr<ID3D12PipelineState>;

    export inline void ThrowIfFailed(HRESULT hr) {
        if (FAILED(hr)) {
            // 抛出异常或处理错误
            //throw std::runtime_error("DirectX operation failed");
        }
    }

};
