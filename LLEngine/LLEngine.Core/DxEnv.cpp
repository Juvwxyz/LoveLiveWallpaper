#include "DxEnv.h"
#include "ComPtr.h"

#include <dxgi1_6.h>
#include <d3d12.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d12.lib")

namespace LLEngine::DxEnv
{
    void Initialize(HWND hwnd)
    {
        HRESULT hr = S_OK;

        ComPtr<ID3D12Device> pDevice;

        // Obtain the DXGI factory
        ComPtr<IDXGIFactory4> dxgiFactory;
        hr = CreateDXGIFactory2(0, LL_IID_PPV_ARGS(&dxgiFactory));

        hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, LL_IID_PPV_ARGS(&pDevice));

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = 1280;
        swapChainDesc.Height = 720;
        swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        swapChainDesc.Scaling = DXGI_SCALING_NONE;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 3;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

        ComPtr<ID3D12CommandQueue> m_CommandQueue;

        D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
        QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        QueueDesc.NodeMask = 1;
        pDevice->CreateCommandQueue(&QueueDesc, LL_IID_PPV_ARGS(&m_CommandQueue));

        ComPtr<IDXGISwapChain1> s_SwapChain1;
        hr = dxgiFactory->CreateSwapChainForHwnd(
            m_CommandQueue.Get(),
            hwnd,
            &swapChainDesc,
            nullptr,
            nullptr,
            &s_SwapChain1
        );

    }
}