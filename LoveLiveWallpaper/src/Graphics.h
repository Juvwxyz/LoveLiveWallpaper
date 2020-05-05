#pragma once

#include "winhead.h"
#include <d2d1_1.h>
#include <d3d11_1.h>
#include <wincodec.h>
#include <wrl.h>
#include <dwrite.h>

using namespace Microsoft::WRL;

namespace LLWP
{
    class WallpaperWindow;

    class Graphics
    {
        friend class Application;

        Graphics() = delete;
        Graphics(const Graphics&) = delete;
        Graphics(const Graphics&&) = delete;
        Graphics& operator=(const Graphics&) = delete;
        Graphics& operator=(const Graphics&&) = delete;
        ~Graphics();
    public:
        static HRESULT Init(const WallpaperWindow& wnd);

        friend HRESULT LoadBitMap(LPCWSTR path, ID2D1Bitmap** bitmap, IWICBitmapSource** source);

        static ComPtr<IWICImagingFactory> WICFactory;

        static ComPtr<ID2D1Bitmap1> targetBitmap;
        static ComPtr<ID2D1Device> D2DDevice;
        static ComPtr<ID2D1DeviceContext> D2DContext;
        static ComPtr<ID2D1Factory1> D2DFactory;
        static ComPtr<ID2D1SolidColorBrush> sBrush;

        static ComPtr<IDWriteFactory> DwriteFactory;
        static ComPtr<IDWriteTextFormat> Dwriteformat;

        static ComPtr<IDWriteFactory> DWFactory;

        static ComPtr<ID3D11Device> D3DDevice;
        static ComPtr<ID3D11DeviceContext> D3DContext;

        static ComPtr<IDXGIAdapter> DXGIAdapter;
        static ComPtr<IDXGIDevice1> DXGIDevice;
        static ComPtr<IDXGIFactory2> DXGIFactory;
        static ComPtr<IDXGISurface> backBuffer;
        static ComPtr<IDXGISwapChain1> DXGISwapChain;

    };
}