#pragma once
#include "Texture.h"

#include <memory>

namespace LLWP
{
    class Sprite : public ::std::enable_shared_from_this<Sprite>
    {
    public:
        Sprite();
        Sprite(const wchar_t* path);

        const Texture& texture() const;
        ::std::shared_ptr<Texture>& texture();


        ~Sprite();
    protected:
        ::std::shared_ptr<Texture> _texture;
        friend class SpriteRenderer;
    };
}