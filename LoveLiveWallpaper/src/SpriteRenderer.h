#pragma once
#include "Renderer.h"
#include "Sprite.h"

namespace LLWP
{
    class SpriteRenderer : public Renderer
    {
    public:
        SpriteRenderer(GameObject& o);
        SpriteRenderer(GameObject& o, const wchar_t* path);

        virtual ~SpriteRenderer();

        Sprite& sprite();

    protected:
        
        Sprite sprite_;

        virtual void Render();
    };

}