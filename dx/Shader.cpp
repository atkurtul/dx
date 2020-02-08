#include <d3d11.h>
#include <d3dcompiler.h>
#include "Shader.h"
#include "Logger.h"
#include "Graphics.h"
#include "std/map.h"

VertexShader::VertexShader()
{
}

VertexShader::VertexShader(const wchar_t* path, const Array<DESCRIPTION>& laylay)
{
	ID3D10Blob* blob;
	log(D3DReadFileToBlob(path, &blob),
		"Failed to load shader: ");
	log(Graphics::device->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(), 0,
		&buffer), "Failed to create vertex shader : ");

	log(Graphics::device->CreateInputLayout(
		(D3D11_INPUT_ELEMENT_DESC*)laylay.data(),
		laylay.size(),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&layout), "Failed to create input layout.");

	blob->Release();
	Graphics::context->VSSetShader(buffer, 0, 0);
	Graphics::context->IASetInputLayout(layout);
}

void VertexShader::Bind()
{
	Graphics::context->VSSetShader(buffer, 0, 0);
	Graphics::context->IASetInputLayout(layout);
}

PixelShader::PixelShader()
{
}

PixelShader::PixelShader(const wchar_t* path)
{
	ID3D10Blob* blob;
	log(D3DReadFileToBlob(path, &blob),
		"Failed to load shader: ");
	log(Graphics::device->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(), 0,
		&buffer), "Failed to create vertex shader : ");
	Graphics::context->PSSetShader(buffer, 0, 0);
	blob->Release();
}

void PixelShader::Bind()
{
	Graphics::context->PSSetShader(buffer, 0, 0);
}
