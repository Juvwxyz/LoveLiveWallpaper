#pragma once
#include "Renderer.h"
#include "Sprite.h"

#include <DirectXMath.h>

namespace LLWP
{
    struct SpriteVertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 texcoord;
        DirectX::XMFLOAT4 color;
    };

    class SpriteRenderer : public Renderer
    {
    public:
        SpriteRenderer(GameObject& o, ::std::shared_ptr<Sprite>&& s);
        SpriteRenderer(GameObject& o, const wchar_t* path);

        virtual ~SpriteRenderer();

        Sprite& sprite();

    protected:
        ComPtr<ID3D11VertexShader> VertexShader;
        ComPtr<ID3D11PixelShader> PixelShader;
        ComPtr<ID3D11Buffer> VertexBuffer;
        ComPtr<ID3D11Buffer> IndexBuffer;
        ComPtr<ID3D11InputLayout> InputLayout;
        ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
        ComPtr<ID3D11SamplerState> SamplerState;
        ::std::shared_ptr<Sprite> _sprite;
        SpriteVertex vtks[4];
        unsigned long indexs[6];

        virtual void Render();
    };

}