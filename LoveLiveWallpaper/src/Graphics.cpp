#include "Graphics.h"

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

    ComPtr<IDXGIAdapter> Graphics::DXGIAdapter;
    ComPtr<IDXGIDevice1> Graphics::DXGIDevice;
    ComPtr<IDXGIFactory2> Graphics::DXGIFactory;
    ComPtr<IDXGISurface> Graphics::backBuffer;
    ComPtr<IDXGISwapChain1> Graphics::DXGISwapChain;

    HRESULT Graphics::Init(const WallpaperWindow& wnd)
    {
        HRESULT hr = S_OK;

        hr = CoInitialize(NULL);

        hr = CoCreateInstance(
            CLSID_WICImagingFactory,
            NULL,
            CLSCTX_INPROC_SERVER,
            IID_PPV_ARGS(&WICFactory)
        );

        hr = D2D1CreateFactory(
            D2D1_FACTORY_TYPE_MULTI_THREADED,
            D2DFactory.GetAddressOf()
        );

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
            D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            featureLevels,
            7,
            D3D11_SDK_VERSION,
            &D3DDevice,
            &r_featureLevel,
            &D3DContext
        );

        hr = D3DDevice.As(&DXGIDevice);

        hr = D2DFactory->CreateDevice(
            DXGIDevice.Get(),
            &D2DDevice
        );

        hr = D2DDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_ENABLE_MULTITHREADED_OPTIMIZATIONS,
            &D2DContext
        );

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

        hr = DXGIAdapter->GetParent(
            IID_PPV_ARGS(&DXGIFactory)
        );

        hr = DXGIFactory->CreateSwapChainForHwnd(
            D3DDevice.Get(),
            wnd.hwnd(),
            &swapChainDesc,
            nullptr,
            nullptr,
            &DXGISwapChain
        );

        hr = DXGIDevice->SetMaximumFrameLatency(1);

        hr = DXGISwapChain->GetBuffer(
            0,
            IID_PPV_ARGS(&backBuffer)
        );

        D2D1_BITMAP_PROPERTIES1 bitmapProp;

        bitmapProp.bitmapOptions = D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW;
        bitmapProp.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
        bitmapProp.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;
        bitmapProp.dpiX = 96.f;
        bitmapProp.dpiY = 96.f;
        bitmapProp.colorContext = nullptr;

        hr = D2DContext->CreateBitmapFromDxgiSurface(
            backBuffer.Get(),
            &bitmapProp,
            &targetBitmap
        );

        D2DContext->SetTarget(targetBitmap.Get());

        hr = D2DContext->CreateSolidColorBrush({ 0.f, 0.f, 0.f, 1.f }, &blackBrush);
        hr = D2DContext->CreateSolidColorBrush({ 1.f, 1.f, 1.f, 1.f }, &whiteBrush);

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            &DwriteFactory
        );

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


        return hr;
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