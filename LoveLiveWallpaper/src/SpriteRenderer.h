#pragma once
#include "Graphics.h"
#include "Renderer.h"
#include "Sprite.h"

#include <DirectXMath.h>

namespace LLWP
{

    class SpriteRenderer : public Renderer
    {
    public:
        SpriteRenderer(GameObject& o, ::std::shared_ptr<Sprite>&& s);
        SpriteRenderer(GameObject& o, const wchar_t* path);

        virtual ~SpriteRenderer();

        Sprite& sprite();

    protected:
        ::std::shared_ptr<Sprite> _sprite;

        virtual void Render();
    };

}