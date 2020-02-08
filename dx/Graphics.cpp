#define WIN32_LEAN_AND_MEAN
#include "Graphics.h"
#include "Logger.h"
#include "Window.h"
#include "Detail.h"
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")

void Graphics::Init()
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = uint(Window::Size.x);
	sd.BufferDesc.Height = uint(Window::Size.y);
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)WINDOWIMPL::WINDOW;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;
	log(D3D11CreateDeviceAndSwapChain(0,
		D3D_DRIVER_TYPE_HARDWARE, 0, 0, 0, 0,
		D3D11_SDK_VERSION,
		&sd,
		&swapChain,
		&device,
		nullptr,
		&context),
		"Failed to create device and swapchain.");

	ID3D11Texture2D* backBuffer;
	log(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer),
		"GetBuffer Failed.");
	log(device->CreateRenderTargetView(backBuffer, 0, &rtarget),
		"Failed to create render target view.");

	context->OMSetRenderTargets(1, &rtarget, 0);
	CD3D11_VIEWPORT viewport(0.f, 0.f, Window::Size.x, Window::Size.y, 0.f, 1.f);
	context->RSSetViewports(1, &viewport);
}

void Graphics::InitDepthBuffer()
{
	//Describe our Depth/Stencil Buffer

	CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, uint(Window::Size.x), uint(Window::Size.y));
	depthStencilTextureDesc.MipLevels = 1;
	depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ID3D11Texture2D* depthStencilBuffer;
	log(device->CreateTexture2D(&depthStencilTextureDesc, NULL, &depthStencilBuffer),
		"Failed to create depth stencil buffer.");

	log(device->CreateDepthStencilView(depthStencilBuffer, NULL, &DSV),
		"Failed to create depth stencil view.");

	context->OMSetRenderTargets(1, &rtarget, DSV);

	//Create depth stencil state
	CD3D11_DEPTH_STENCIL_DESC depthstencildesc(D3D11_DEFAULT);
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	ID3D11DepthStencilState* depthStencilState;
	log(device->CreateDepthStencilState(&depthstencildesc, &depthStencilState),
		"Failed to create depth stencil state.");

	CD3D11_DEPTH_STENCIL_DESC depthstencildesc_drawMask(D3D11_DEFAULT);
	depthstencildesc_drawMask.DepthEnable = FALSE;
	depthstencildesc_drawMask.StencilEnable = TRUE;

	depthstencildesc_drawMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	depthstencildesc_drawMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_drawMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_drawMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

	depthstencildesc_drawMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_ALWAYS;
	depthstencildesc_drawMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_drawMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_drawMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_INCR_SAT;

	ID3D11DepthStencilState* depthStencilState_drawMask;
	log(device->CreateDepthStencilState(&depthstencildesc_drawMask, &depthStencilState_drawMask),
		"Failed to create depth stencil state for drawing mask.");

	CD3D11_DEPTH_STENCIL_DESC depthstencildesc_applyMask(D3D11_DEFAULT);
	depthstencildesc_applyMask.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	depthstencildesc_applyMask.StencilEnable = TRUE;

	depthstencildesc_applyMask.BackFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	depthstencildesc_applyMask.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_applyMask.BackFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_applyMask.BackFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

	depthstencildesc_applyMask.FrontFace.StencilFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	depthstencildesc_applyMask.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_applyMask.FrontFace.StencilFailOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;
	depthstencildesc_applyMask.FrontFace.StencilPassOp = D3D11_STENCIL_OP::D3D11_STENCIL_OP_KEEP;

	ID3D11DepthStencilState* depthStencilState_applyMask;
	log(device->CreateDepthStencilState(&depthstencildesc_applyMask, &depthStencilState_applyMask),
		"Failed to create depth stencil state for applying mask.");

	ID3D11RasterizerState* rasterizerState;
	//Create Rasterizer State
	CD3D11_RASTERIZER_DESC rasterizerDesc(D3D11_DEFAULT);
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	log(device->CreateRasterizerState(&rasterizerDesc, &rasterizerState),
		"Failed to create rasterizer state.");
	Blend();
	RasterizeState();
}

void Graphics::Blend()
{
	D3D11_BLEND_DESC desc{};
	desc.AlphaToCoverageEnable					= true;
	desc.IndependentBlendEnable					= true;
	desc.RenderTarget[0].BlendEnable			= true;
	desc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha			= D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&desc, &blend);
	context->OMSetBlendState(blend, 0, 0xffffffff);
}

void Graphics::RasterizeState()
{
	D3D11_RASTERIZER_DESC desc{};
	desc.FillMode					= D3D11_FILL_SOLID;
	desc.CullMode					= D3D11_CULL_BACK;
	desc.FrontCounterClockwise		= false;
	desc.DepthBias					= 0;
	desc.DepthBiasClamp				= 0;
	desc.SlopeScaledDepthBias		= 0.f;
	desc.DepthClipEnable			= true;
	desc.ScissorEnable				= false;
	desc.MultisampleEnable			= false;
	desc.AntialiasedLineEnable		= false;
	device->CreateRasterizerState(&desc, &fill);
	context->RSSetState(fill);
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	device->CreateRasterizerState(&desc, &wireframe);
}

void Graphics::Fill()
{
	context->RSSetState(fill);
}

void Graphics::Wireframe()
{
	context->RSSetState(wireframe);
}

void Graphics::SetTopology(int topology)
{
	context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY(topology));
}

void Graphics::Clear()
{
	context->ClearRenderTargetView(rtarget, color);
	context->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH , 1.0f, 0);
}

void Graphics::ClearColor(float r, float g, float b, float a)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;
}

void Graphics::SwapBuffers(int sync)
{
	swapChain->Present(sync, 0);
}

void Graphics::SetRTarget()
{
	context->OMSetRenderTargets(1, &rtarget, 0);
}

void Graphics::CreateUniformBuffer(unsigned byteWidth, void* memptr, unsigned slot)
{
	static D3D11_BUFFER_DESC	DESC = {
		0, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	static D3D11_SUBRESOURCE_DATA SUBDATA = { 0, 0, 0 };
	DESC.ByteWidth = byteWidth;
	SUBDATA.pSysMem = memptr;
	ID3D11Buffer* buffer;
	log(Graphics::device->CreateBuffer(&DESC, &SUBDATA, &buffer),
		"Failed to create constant buffer.");
	Graphics::context->VSSetConstantBuffers(slot, 1, &buffer);
	buffer->Release();
}


