#include "SpriteRenderer.h"

#include "Graphics.h"
#include "Sprite.h"
#include "Transform.h"


namespace LLWP
{
    SpriteRenderer::SpriteRenderer(GameObject& o) :
        Renderer(o),
        sprite_()
    {
    }

    SpriteRenderer::SpriteRenderer(GameObject& o, const wchar_t* path) :
        Renderer(o),
        sprite_(path)
    {
    }

    SpriteRenderer::~SpriteRenderer()
    {
    }
    Sprite& SpriteRenderer::sprite()
    {
        return sprite_;
    }
    void SpriteRenderer::Render()
    {
        if (sprite_.texture_->bitmap.Get() == nullptr)
        {
            Graphics::D2DContext->DrawRectangle(
                D2D1_RECT_F{ 0 },
                Graphics::sBrush.Get()
            );
        }
        else
        {
            Graphics::D2DContext->DrawBitmap(
                sprite_.texture_->bitmap.Get(),
                this->transform().rect(),
                1.f,
                D2D1_INTERPOLATION_MODE_LINEAR,
                nullptr,
                &this->transform().localToWorld()
            );
        }
    }
}