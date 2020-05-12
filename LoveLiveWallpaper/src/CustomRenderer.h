#pragma once
#include "Graphics.h"
#include "Renderer.h"

#include <DirectXMath.h>

namespace LLWP
{
    struct VSMatrixBuffer
    {
        DirectX::XMMATRIX tranMat;
        DirectX::XMMATRIX viewMat;
        DirectX::XMMATRIX projMat;
    };

    struct SpriteVertex
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT2 texcoord;
        DirectX::XMFLOAT4 color;
    };

    class CustomRenderer : public Renderer
    {
    public:
        CustomRenderer(
            GameObject& obj,
            const wchar_t* path,
            DirectX::XMFLOAT4 color = DirectX::XMFLOAT4{1,1,1,1}
        );

        HRESULT LoadTextureFromFile(const wchar_t* path);

    protected:
        ComPtr<ID3D11VertexShader> VertexShader;
        ComPtr<ID3D11PixelShader> PixelShader;
        ComPtr<ID3D11InputLayout> InputLayout;

        ComPtr<ID3D11Buffer> VertexBuffer;
        ComPtr<ID3D11Buffer> IndexBuffer;
        ComPtr<ID3D11Buffer> MatrixBuffer;
        D3D11_MAPPED_SUBRESOURCE mappedMatrixBuffer;
        ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
        ComPtr<ID3D11SamplerState> SamplerState;
        ComPtr<ID3D11Texture2D> texture;
        SpriteVertex vtks[4];
        unsigned long indexs[6];
        virtual void Render();

        UINT size_x, size_y;
    };
}