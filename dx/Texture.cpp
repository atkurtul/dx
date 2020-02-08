#include "Texture.h"
#include <d3d11.h>
#include <DirectXTex.h>
#include "Logger.h"

Texture::Texture() :
	texture(nullptr),
	view(nullptr)
{
}

Texture::Texture(const char* path)
{
	DirectX::ScratchImage scratch;
	log(DirectX::LoadFromWICFile(wstr(path), DirectX::WIC_FLAGS_IGNORE_SRGB, nullptr, scratch),
		"Failed to load texture.");
	width = scratch.GetMetadata().width;
	height = scratch.GetMetadata().height;
	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = scratch.GetMetadata().width;
	textureDesc.Height = scratch.GetMetadata().height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
	log(Graphics::device->CreateTexture2D(&textureDesc, nullptr, &texture),
		"Failed to create texture.");

	// write image data into top mip level
	Graphics::context->UpdateSubresource(
		texture, 0u, nullptr, scratch.GetPixels(), scratch.GetMetadata().width * 4, 0u);

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	log(Graphics::device->CreateShaderResourceView(texture, &srvDesc, &view),
		"Failed to create texture view.");
	Graphics::context->GenerateMips(view);
}

void Texture::Bind(unsigned slot)
{
	Graphics::context->PSSetShaderResources(slot, 1, &view);
}


class Sampler
{
public:
	ID3D11SamplerState* sampler;
	Sampler()
	{
		D3D11_SAMPLER_DESC samplerDesc = CD3D11_SAMPLER_DESC{ CD3D11_DEFAULT{} };
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;

		log(Graphics::device->CreateSamplerState(&samplerDesc, &sampler), "Failed to create sampler");
	}

	void Bind()
	{
		Graphics::context->PSSetSamplers(0, 1, &sampler);
	}
};