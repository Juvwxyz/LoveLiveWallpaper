#pragma once
#include "Texture.h"

namespace LLWP
{
    class Sprite
    {
    public:
        Sprite();
        Sprite(const wchar_t* path);

        Texture& texture();

        ~Sprite();
    protected:
        Texture* texture_;
        friend class SpriteRenderer;
    };
}