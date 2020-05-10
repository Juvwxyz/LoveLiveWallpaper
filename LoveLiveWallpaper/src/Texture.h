#pragma once
#include "Graphics.h"
#include "typedef.h"

#include <memory>

namespace LLWP
{
    class Texture : public ::std::enable_shared_from_this<Texture>
    {
    public:
        Texture();
        Texture(const wchar_t* path);

        HRESULT LoadFromFile(LPCWSTR path);

        virtual ~Texture();

    protected:
        ComPtr<ID2D1Bitmap> bitmap;
        ComPtr<ID2D1Bitmap1> bitmap1;
        ComPtr<IWICBitmapSource> source;

        ComPtr<ID3D11Texture2D> texture;

        BYTE* alphaBuffer;
        SIZE pixelSize;

        friend class SpriteRenderer;
    };
}