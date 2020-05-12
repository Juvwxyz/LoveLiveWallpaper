#include "Graphics.h"

#include "Exception.h"
#include "WallpaperWindow.h"

namespace LLWP
{
    Graphics::~Graphics()
    {
    }
    ComPtr<IWICImagingFactory> Graphics::WICFactory;

    ComPtr<ID2D1Bitmap1> Graphics::targetBitmap;
    ComPtr<ID2D1Device> Graphics::D2DDevice;
    ComPtr<ID2D1DeviceContext> Graphics::D2DContext;
    ComPtr<ID2D1Factory1> Graphics::D2DFactory;

    ComPtr<ID2D1SolidColorBrush> Graphics::blackBrush;
    ComPtr<ID2D1SolidColorBrush> Graphics::whiteBrush;

    ComPtr<IDWriteFactory> Graphics::DwriteFactory;
    ComPtr<IDWriteTextFormat> Graphics::Dwriteformat;

    ComPtr<ID3D11Device> Graphics::D3DDevice;
    ComPtr<ID3D11DeviceContext> Graphics::D3DContext;
    ComPtr<ID3D11RenderTargetView> Graphics::D3DRenderTargetView;
    ComPtr<ID3D11BlendState> Graphics::D3DBlendState;

    ComPtr<IDXGIAdapter> Graphics::DXGIAdapter;
    ComPtr<IDXGIDevice1> Graphics::DXGIDevice;
    ComPtr<IDXGIFactory2> Graphics::DXGIFactory;
    ComPtr<IDXGISurface> Graphics::backBufferSurface;
    ComPtr<ID3D11Texture2D> Graphics::backBufferTexture;
    ComPtr<IDXGISwapChain1> Graphics::DXGISwapChain;

    HRESULT Graphics::Init(const WallpaperWindow& wnd)
    {
        HRESULT hr = S_OK;

        hr = CoInitialize(NULL);
        if (hr != S_OK) return hr;

        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&WICFactory)
        );
        if (hr != S_OK) return hr;

        hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_MULTI_THREADED,
            D2DFactory.GetAddressOf()
        );
        if (hr != S_OK) return hr;

        D3D_FEATURE_LEVEL featureLevels[]{
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        D3D_FEATURE_LEVEL r_featureLevel;

        hr = D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_DEBUG,
            featureLevels,
            7,
            D3D11_SDK_VERSION,
            &D3DDevice,
            &r_featureLevel,
            &D3DContext
        );
        if (hr != S_OK) return hr;


        hr = D3DDevice.As(&DXGIDevice);
        if (hr != S_OK) return hr;

        hr = D2DFactory->CreateDevice(
            DXGIDevice.Get(),
            &D2DDevice
        );
        if (hr != S_OK) return hr;

        hr = D2DDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
            &D2DContext
        );
        if (hr != S_OK) return hr;

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
        swapChainDesc.Width = 0;
        swapChainDesc.Height = 0;
        swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        swapChainDesc.Stereo = false;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = 2;
        swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
        swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        swapChainDesc.Flags = 0;
        swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

        hr = DXGIDevice->GetAdapter(&DXGIAdapter);
        if (hr != S_OK) return hr;

        hr = DXGIAdapter->GetParent(
            IID_PPV_ARGS(&DXGIFactory)
        );
        if (hr != S_OK) return hr;

        hr = DXGIFactory->CreateSwapChainForHwnd(
            D3DDevice.Get(),
            wnd.hwnd(),
            &swapChainDesc,
            nullptr,
            nullptr,
            &DXGISwapChain
        );
        if (hr != S_OK) return hr;

        hr = DXGIDevice->SetMaximumFrameLatency(1);
        if (hr != S_OK) return hr;

        hr = DXGISwapChain->GetBuffer(
            0,
            IID_PPV_ARGS(&backBufferSurface)
        );
        if (hr != S_OK) return hr;

        hr = DXGISwapChain->GetBuffer(
            0,
            __uuidof(ID3D11Texture2D),
            &backBufferTexture
        );
        if (hr != S_OK) return hr;

        hr = D3DDevice->CreateRenderTargetView(
            backBufferTexture.Get(),
            nullptr,
            &D3DRenderTargetView
        );
        if (hr != S_OK) return hr;

        D3D11_VIEWPORT vp;
        vp.Width = float(1920);
        vp.Height = float(1080);
        vp.MinDepth = 0.0f;
        vp.MaxDepth = 1.0f;
        vp.TopLeftX = 0.0f;
        vp.TopLeftY = 0.0f;
        D3DContext->RSSetViewports(1, &vp);

        D3D11_BLEND_DESC blendDesc;
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.IndependentBlendEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        hr = D3DDevice->CreateBlendState(&blendDesc, &D3DBlendState);
        if (hr != S_OK) return hr;

        D3DContext->OMSetBlendState(D3DBlendState.Get(), nullptr, 0xffffffff);

        D2D1_BITMAP_PROPERTIES1 bitmapProp;

        bitmapProp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
        bitmapProp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        bitmapProp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
        bitmapProp.dpiX = 96.f;
        bitmapProp.dpiY = 96.f;
        bitmapProp.colorContext = nullptr;

        hr = D2DContext->CreateBitmapFromDxgiSurface(
            backBufferSurface.Get(),
            &bitmapProp,
            &targetBitmap
        );
        if (hr != S_OK) return hr;

        D2DContext->SetTarget(targetBitmap.Get());

        hr = D2DContext->CreateSolidColorBrush({ 0.f, 0.f, 0.f, 1.f }, &blackBrush);
        if (hr != S_OK) return hr;
        hr = D2DContext->CreateSolidColorBrush({ 1.f, 1.f, 1.f, 1.f }, &whiteBrush);
        if (hr != S_OK) return hr;

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            &DwriteFactory
        );
        if (hr != S_OK) return hr;

        hr = DwriteFactory->CreateTextFormat(
            L"Input",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            24.f,
            L"",
            &Dwriteformat
        );
        if (hr != S_OK) return hr;
        return S_OK;
    }

    HRESULT LoadBitMap(LPCWSTR path, ID2D1Bitmap** ppBitmap, IWICBitmapSource** source)
    {
        HRESULT hr = S_OK;

        ComPtr<IWICBitmapDecoder> pDecoder;
        ComPtr<IWICBitmapFrameDecode> pSource;
        //ComPtr<IWICFormatConverter> pConverter;

        hr = Graphics::WICFactory->
            CreateDecoderFromFilename(
                path,
                NULL,
                GENERIC_READ,
                WICDecodeMetadataCacheOnLoad,
                pDecoder.GetAddressOf()
            );

        hr = pDecoder->GetFrame(0, &pSource);

        hr = Graphics::WICFactory->
            CreateFormatConverter((IWICFormatConverter**)source);

        hr = ((IWICFormatConverter*)(*source))->Initialize(
            pSource.Get(),
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeCustom
        );

        hr = Graphics::D2DContext->
            CreateBitmapFromWicBitmap(
                ((IWICFormatConverter*)(*source)),
                NULL,
                ppBitmap
            );

        return hr;
    }

}