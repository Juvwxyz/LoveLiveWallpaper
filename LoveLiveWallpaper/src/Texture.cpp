#include "Texture.h"

namespace LLWP
{

    Texture::Texture() :
		alphaBuffer(nullptr),
		pixelSize{ 1,1 },
		bitmap(nullptr),
		source(nullptr)
    {
    }

    Texture::Texture(const wchar_t* path):
		alphaBuffer(nullptr),
		pixelSize{ 1,1 },
		bitmap(nullptr),
		source(nullptr)
    {
		LoadFromFile(path);
    }

	HRESULT Texture::LoadFromFile(LPCWSTR path)
    {
		HRESULT hr = S_OK;

		ComPtr<IWICBitmapDecoder> pDecoder;
		ComPtr<IWICBitmapFrameDecode> pSource;

		hr = Graphics::WICFactory->
			CreateDecoderFromFilename(
				path,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				pDecoder.GetAddressOf()
			);

		if (hr != S_OK) return hr;

		hr = pDecoder->GetFrame(0, &pSource);

		if (hr != S_OK) return hr;

		hr = Graphics::WICFactory->
			CreateFormatConverter((IWICFormatConverter**)(source.ReleaseAndGetAddressOf()));

		if (hr != S_OK) return hr;

		hr = ((IWICFormatConverter*)(source.Get()))->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom
		);

		if (hr != S_OK) return hr;

		hr = Graphics::D2DContext->
			CreateBitmapFromWicBitmap(
				((IWICFormatConverter*)(source.Get())),
				NULL,
				&bitmap
			);

		if (hr != S_OK) return hr;

		if (alphaBuffer != nullptr)
		{
			delete[] alphaBuffer;
		}

		source->GetSize((UINT*)&pixelSize.x, (UINT*)&pixelSize.y);
		BYTE* buffer = new BYTE[(UINT64)pixelSize.x * pixelSize.y * 4];
		alphaBuffer = new BYTE[(UINT64)pixelSize.x * pixelSize.y];
		source->CopyPixels(
			nullptr,
			pixelSize.x * 4,
			pixelSize.x * pixelSize.y * 4,
			buffer
		);
		for (size_t y = 0; y < pixelSize.y; y++)
		{
			for (size_t x = 0; x < pixelSize.x; x++)
			{
				alphaBuffer[y * pixelSize.x + x] = buffer[x * pixelSize.y * 4 + y * 4 + 3];
			}
		}

		D3D11_TEXTURE2D_DESC sysTexDesc;
		sysTexDesc.Width = pixelSize.x;
		sysTexDesc.Height = pixelSize.y;
		sysTexDesc.MipLevels = 1;
		sysTexDesc.ArraySize = 1;
		sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sysTexDesc.SampleDesc.Count = 1;
		sysTexDesc.SampleDesc.Quality = 0;
		sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
		sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		sysTexDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA data;

		data.pSysMem = buffer;
		data.SysMemPitch = pixelSize.x * 4;
		data.SysMemSlicePitch = 0;

		hr = Graphics::D3DDevice->CreateTexture2D(&sysTexDesc, &data, &texture);

		D2D1_BITMAP_PROPERTIES1 prop;
		prop.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		prop.pixelFormat.alphaMode = D2D1_ALPHA_MODE_STRAIGHT;
		prop.dpiX = 96;
		prop.dpiY = 96;
		prop.bitmapOptions = D2D1_BITMAP_OPTIONS_CPU_READ;

		ComPtr<IDXGISurface> surf;
		texture->QueryInterface(surf.GetAddressOf());
		
		hr = Graphics::D2DContext->CreateBitmapFromDxgiSurface(surf.Get(), prop, &bitmap1);

		delete[] buffer;
		return hr;
    }

    Texture::~Texture()
    {
		delete[] alphaBuffer;
    }

}

