#pragma once
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
class Graphics
{
public:
	inline static ID3D11Device*					device		= nullptr;
	inline static ID3D11DeviceContext*			context		= nullptr;
	inline static IDXGISwapChain*				swapChain	= nullptr;
	inline static ID3D11RenderTargetView*		rtarget		= nullptr;
	inline static ID3D11DepthStencilView*		DSV			= nullptr;
	inline static ID3D11RasterizerState*		fill		= nullptr;
	inline static ID3D11RasterizerState*		wireframe	= nullptr;
	inline static ID3D11BlendState*				blend		= nullptr;
	friend class VertexArray;
	template<class T>
	friend class Uniform;
	friend class VertexShader;
	friend class PixelShader;
	inline static float color[4];

	static void Init();
	static void InitDepthBuffer();
	static void Blend();
	static void RasterizeState();
	static void Fill();
	static void Wireframe();
	static void SetTopology(int);
	static void Clear();
	static void ClearColor(float r, float g, float b, float a);
	static void SwapBuffers(int sync);
	static void SetRTarget();
	static void CreateUniformBuffer(unsigned byteWidth, void* memptr, unsigned slot);
};