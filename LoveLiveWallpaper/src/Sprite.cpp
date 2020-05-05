#include "Sprite.h"
#include "SpriteRenderer.h"
#include "Texture.h"

namespace LLWP
{
    Sprite::Sprite() :
        texture_(new Texture())
    {

    }
    Sprite::Sprite(const wchar_t* path) :
        texture_(new Texture(path))
    {
    }
    Texture& Sprite::texture()
    {
        return *texture_;
    }
    Sprite::~Sprite()
    {
        delete texture_;
    }
}