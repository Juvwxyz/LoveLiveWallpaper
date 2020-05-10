#include "SpriteRenderer.h"

#include "Graphics.h"
#include "Sprite.h"
#include "Transform.h"

#include "SpriteVertexShader.shh"
#include "SpritePixelShader.shh"

using ::std::shared_ptr;
using ::std::make_shared;

namespace LLWP
{
    SpriteRenderer::SpriteRenderer(GameObject& o, shared_ptr<Sprite>&& s) :
        Renderer(o),
        _sprite(s),
        vtks{
            {{-1, 1,0},{0,0},{1,1,1,1}},
            {{ 1, 1,0},{1,0},{1,1,1,1}},
            {{ 1,-1,0},{0,1},{1,1,1,1}},
            {{-1,-1,0},{1,1},{1,1,1,1}}
        },
        indexs{ 0,1,2,1,2,3 }
    {
    }

    SpriteRenderer::SpriteRenderer(GameObject& o, const wchar_t* path) :
        Renderer(o),
        _sprite(make_shared<Sprite>(path)),
        vtks{
            {{-1, 1,0},{0,0},{1,1,1,1}},
            {{ 1, 1,0},{1,0},{1,1,1,1}},
            {{ 1,-1,0},{0,1},{1,1,1,1}},
            {{-1,-1,0},{1,1},{1,1,1,1}}
        },
        indexs{ 0,1,2,1,2,3 }
    {
        HRESULT hr = S_OK;

        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = vtks;

        hr = Graphics::D3DDevice->CreateBuffer(&bd, &initData, &VertexBuffer);

        D3D11_BUFFER_DESC IBD = {};
        IBD.Usage = D3D11_USAGE_DEFAULT;
        IBD.ByteWidth = sizeof(unsigned long) * 6;
        IBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
        IBD.CPUAccessFlags = 0;
        bd.MiscFlags = 0;
        D3D11_SUBRESOURCE_DATA IData = {};
        IData.pSysMem = indexs;

        hr = Graphics::D3DDevice->CreateBuffer(&IBD, &IData, &IndexBuffer);

        const D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            { "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
            { "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,20,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0 }
        };

        unsigned int a = sizeof(SpriteVertexShaderBytecode);

        hr = Graphics::D3DDevice->CreateInputLayout(
            ied, 3,
            SpriteVertexShaderBytecode,
            a,
            &InputLayout);

        hr = Graphics::D3DDevice->CreateVertexShader(
            SpriteVertexShaderBytecode,
            sizeof(SpriteVertexShaderBytecode),
            nullptr,
            &VertexShader
        );

        hr = Graphics::D3DDevice->CreatePixelShader(
            SpritePixelShaderBytecode,
            sizeof(SpritePixelShaderBytecode),
            nullptr,
            &PixelShader
        );

        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
        srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MipLevels = 1;

        hr = Graphics::D3DDevice->CreateShaderResourceView(_sprite->_texture->texture.Get(), &srvDesc, &ShaderResourceView);
    
        D3D11_SAMPLER_DESC sampDesc;
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
        sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        sampDesc.MipLODBias = 0;
        sampDesc.MaxAnisotropy = 0;
        sampDesc.MinLOD = 0;
        sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
        hr = Graphics::D3DDevice->CreateSamplerState(&sampDesc, &SamplerState);

    
    }

    SpriteRenderer::~SpriteRenderer()
    {
    }

    Sprite& SpriteRenderer::sprite()
    {
        return *_sprite;
    }

    void SpriteRenderer::Render()
    {
        Graphics::D3DContext->IASetInputLayout(InputLayout.Get());
        Graphics::D3DContext->VSSetShader(VertexShader.Get(), nullptr, 0u);
        Graphics::D3DContext->PSSetShader(PixelShader.Get(), nullptr, 0u);
        Graphics::D3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        const UINT stride = sizeof(SpriteVertex);
        const UINT offset = 0u;
        Graphics::D3DContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
        Graphics::D3DContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        Graphics::D3DContext->PSSetShaderResources(0, 1, ShaderResourceView.GetAddressOf());
        Graphics::D3DContext->PSSetSamplers(0, 1, SamplerState.GetAddressOf());
        Graphics::D3DContext->DrawIndexed(6, 0, 0);

        //if (_sprite->_texture->bitmap.Get() == nullptr)
        //{
        //    Graphics::D2DContext->FillRectangle(
        //        this->transform().rect(),
        //        Graphics::whiteBrush.Get()
        //    );
        //}
        //else
        //{
        //    Graphics::D2DContext->DrawBitmap(
        //        _sprite->_texture->bitmap.Get(),
        //        this->transform().rect(),
        //        1.f,
        //        D2D1_INTERPOLATION_MODE_LINEAR,
        //        nullptr,
        //        &this->transform().localToWorld()
        //    );
        //}
    }
}