#pragma once
#include "Graphics.h"
#include "typedef.h"

namespace LLWP
{
    class Texture
    {
    public:
        Texture();
        Texture(const wchar_t* path);

        void LoadFromFile(LPCWSTR path);

        virtual ~Texture();

    protected:
        ComPtr<ID2D1Bitmap> bitmap;
        ComPtr<IWICBitmapSource> source;

        BYTE* alphaBuffer;
        SIZE pixelSize;

        friend class SpriteRenderer;
    };
}