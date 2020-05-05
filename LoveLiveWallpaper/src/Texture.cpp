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

	void Texture::LoadFromFile(LPCWSTR path)
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

		hr = pDecoder->GetFrame(0, &pSource);

		hr = Graphics::WICFactory->
			CreateFormatConverter((IWICFormatConverter**)source.GetAddressOf());

		hr = ((IWICFormatConverter*)(source.Get()))->Initialize(
			pSource.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeCustom
		);

		hr = Graphics::D2DContext->
			CreateBitmapFromWicBitmap(
				((IWICFormatConverter*)(source.Get())),
				NULL,
				&bitmap
			);

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
		delete[] buffer;
    }

    Texture::~Texture()
    {
		delete[] alphaBuffer;
    }

}

