#include "Texture.h"

namespace LLWP
{

    Texture::Texture() :
		alphaBuffer(nullptr),
		pixelSize{ 1,1 },
		bitmap(nullptr)
    {
    }

    Texture::Texture(const wchar_t* path):
		alphaBuffer(nullptr),
		pixelSize{ 1,1 },
		bitmap(nullptr)
    {
		LoadFromFile(path);
    }

	HRESULT Texture::LoadFromFile(LPCWSTR path)
    {
		HRESULT hr = S_OK;

		ComPtr<IWICBitmapDecoder> decoder;
		ComPtr<IWICBitmapFrameDecode> frameDecode;

		hr = Graphics::WICFactory->
			CreateDecoderFromFilename(
				path,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				decoder.GetAddressOf()
			);

		if (hr != S_OK) return hr;

		hr = decoder->GetFrame(0, &frameDecode);

		if (hr != S_OK) return hr;

		ComPtr<IWICFormatConverter> formatConverter;

		hr = Graphics::WICFactory->
			CreateFormatConverter(&formatConverter);

		if (hr != S_OK) return hr;

		hr = formatConverter->Initialize(
			frameDecode.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom
		);

		if (hr != S_OK) return hr;

		hr = Graphics::D2DContext->
			CreateBitmapFromWicBitmap(
				formatConverter.Get(),
				NULL,
				&bitmap
			);

		if (hr != S_OK) return hr;

		if (alphaBuffer != nullptr)
		{
			delete[] alphaBuffer;
		}

		formatConverter->GetSize((UINT*)&pixelSize.x, (UINT*)&pixelSize.y);
		BYTE* buffer = new BYTE[(UINT64)pixelSize.x * pixelSize.y * 4];
		alphaBuffer = new BYTE[(UINT64)pixelSize.x * pixelSize.y];
		formatConverter->CopyPixels(
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

		delete[] buffer;
		return hr;
    }

    Texture::~Texture()
    {
		delete[] alphaBuffer;
    }

}

