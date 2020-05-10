#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Texture.h"

namespace LLWP
{
    Sprite::Sprite() :
        _texture(nullptr)
    {

    }

    Sprite::Sprite(const wchar_t* path) :
        _texture(::std::make_shared<Texture>(path))
    {
    }

    const Texture& Sprite::texture() const
    {
        return *_texture;
    }

    ::std::shared_ptr<Texture>& Sprite::texture()
    {
        return _texture;
    }

    Sprite::~Sprite()
    {

    }
}