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

        BYTE* alphaBuffer;
        SIZE pixelSize;

        D2D1_VECTOR_4F color;

        friend class SpriteRenderer;
    };
}