#include "CustomRenderer.h"

#include "Transform.h"

#include "SpriteVertexShader.shh"
#include "SpritePixelShader.shh"

namespace LLWP
{
    CustomRenderer::CustomRenderer(GameObject& obj, const wchar_t* path, DirectX::XMFLOAT4 color) :
        Renderer(obj),
        vtks{
            {{-1, 1,0},{0,0}}, // top-left
            {{ 1, 1,0},{1,0}}, // top-right
            {{ 1,-1,0},{1,1}}, // bottom-right
            {{-1,-1,0},{0,1}}  // bottom-left
    },
        indexs{ 0,1,2,0,2,3 },
        _color(color)
    {
        HRESULT hr = S_OK;

        hr = LoadTextureFromFile(path);

        float x = (float)size_x;
        float y = (float)size_y;

        vtks[0].pos = { -x / 2,  y / 2, 0.5f };
        vtks[1].pos = {  x / 2,  y / 2, 0.5f };
        vtks[2].pos = {  x / 2, -y / 2, 0.5f };
        vtks[3].pos = { -x / 2, -y / 2, 0.5f };

        VSCBuffer initbuffer;
        initbuffer.color = { 1,1,1,1 };

        D3D11_SUBRESOURCE_DATA initData;
        initData.pSysMem = &initbuffer;

        D3D11_BUFFER_DESC MBDesc;
        MBDesc.Usage = D3D11_USAGE_DYNAMIC;
        MBDesc.ByteWidth = sizeof(VSCBuffer);
        MBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        MBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        MBDesc.MiscFlags = 0;
        MBDesc.StructureByteStride = 0;
        hr = Graphics::D3DDevice->CreateBuffer(&MBDesc, &initData, &vscBuffer);

        D3D11_BUFFER_DESC bd;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SpriteVertex) * 4;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;
        bd.MiscFlags = 0;

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
            { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
        };

        hr = Graphics::D3DDevice->CreateInputLayout(
            ied, 2,
            SpriteVertexShaderBytecode,
            sizeof(SpriteVertexShaderBytecode),
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

        hr = Graphics::D3DDevice->CreateShaderResourceView(texture.Get(), &srvDesc, &ShaderResourceView);

        D3D11_SAMPLER_DESC sampDesc;
        sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//D3D11_FILTER_MIN_MAG_MIP_POINT;
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

    HRESULT CustomRenderer::LoadTextureFromFile(const wchar_t* path)
    {
        HRESULT hr = S_OK;

        ComPtr<IWICBitmapDecoder> pDecoder;
        ComPtr<IWICBitmapFrameDecode> pSource;

        hr = Graphics::WICFactory->
            CreateDecoderFromFilename(
                path,
                NULL,
                GENERIC_READ,
                WICDecodeMetadataCacheOnLoad,
                pDecoder.GetAddressOf()
            );

        if (hr != S_OK) return hr;

        hr = pDecoder->GetFrame(0, &pSource);

        if (hr != S_OK) return hr;

        ComPtr<IWICFormatConverter> converter;

        hr = Graphics::WICFactory->
            CreateFormatConverter(&converter);

        if (hr != S_OK) return hr;

        hr = converter->Initialize(
            pSource.Get(),
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeCustom
        );

        if (hr != S_OK) return hr;

        converter->GetSize(&size_x, &size_y);

        DWORD* buffer = new DWORD[(uint64_t)sizeof(DWORD) * size_x * size_y];
        alphaBuffer = new BYTE[(uint64_t)size_x * size_y];
        converter->CopyPixels(
            nullptr,
            size_x * 4,
            size_x * size_y * 4,
            reinterpret_cast<BYTE*>(buffer)
        );

        for (size_t y = 0; y < size_y; y++)
        {
            for (size_t x = 0; x < size_x; x++)
            {
                alphaBuffer[y * size_x + x] = (BYTE)(buffer[(y * size_x + x)]>>24);
            }
        }

        D3D11_TEXTURE2D_DESC texDesc;
        texDesc.Width = size_x;
        texDesc.Height = size_y;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        texDesc.SampleDesc.Count = 1;
        texDesc.SampleDesc.Quality = 0;
        texDesc.Usage = D3D11_USAGE_DYNAMIC;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        texDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA data;

        data.pSysMem = buffer;
        data.SysMemPitch = size_x * 4;
        data.SysMemSlicePitch = 0;

        hr = Graphics::D3DDevice->CreateTexture2D(&texDesc, &data, &texture);

        delete[] buffer;
        return hr;
    }

    void CustomRenderer::setColor(float r, float g, float b, float a)
    {
        _color = { r,g,b,a };
    }

    byte CustomRenderer::getAlpha(int x, int y)
    {
        return alphaBuffer[y * size_x + x];
    }

    void CustomRenderer::Render()
    {
        HRESULT hr = S_OK;
        hr = Graphics::D3DContext->Map(vscBuffer.Get(), 0u,
            D3D11_MAP_WRITE_DISCARD, 0u, &mappedVSCBuffer);

        VSCBuffer* buffer = (VSCBuffer*)mappedVSCBuffer.pData;

        DirectX::XMMATRIX mat =
        transform().localToWorld()
        *
        DirectX::XMMATRIX
        {
            1/960.f,       0,0,0,
                  0,1/540.f,0,0,
                  0,       0,1,0,
                  0,       0,0,1
        };

        buffer->tranMat = DirectX::XMMatrixTranspose(mat);
        buffer->color = _color;

        Graphics::D3DContext->Unmap(vscBuffer.Get(), 0u);

        Graphics::D3DContext->VSSetConstantBuffers(0, 1, vscBuffer.GetAddressOf());
        Graphics::D3DContext->IASetInputLayout(InputLayout.Get());
        Graphics::D3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        const UINT stride = sizeof(SpriteVertex);
        const UINT offset = 0u;
        Graphics::D3DContext->IASetVertexBuffers(0, 1, VertexBuffer.GetAddressOf(), &stride, &offset);
        Graphics::D3DContext->IASetIndexBuffer(IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        Graphics::D3DContext->VSSetShader(VertexShader.Get(), nullptr, 0u);
        Graphics::D3DContext->PSSetShader(PixelShader.Get(), nullptr, 0u);
        Graphics::D3DContext->PSSetShaderResources(0, 1, ShaderResourceView.GetAddressOf());
        Graphics::D3DContext->PSSetSamplers(0, 1, SamplerState.GetAddressOf());
        Graphics::D3DContext->DrawIndexed(6, 0, 0);

    }

}
