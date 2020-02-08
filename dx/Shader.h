#pragma once
#include "std/map.h"
#define DEBUG_VS L"..\\x64\\Debug\\VertexShader.cso"
#define DEBUG_PS L"..\\x64\\Debug\\PixelShader.cso"
#define RELEASE_VS L"..\\x64\\Release\\VertexShader.cso"
#define RELEASE_PS L"..\\x64\\Release\\PixelShader.cso"
using uint = unsigned;

struct DESCRIPTION
{
	const char*					SemanticName;
	uint						SemanticIndex;
	int							Format;
	uint						InputSlot;
	uint						AlignedByteOffset;
	int							InputSlotClass;
	uint						InstanceDataStepRate;
};

template<uint stride, class type = float, uint slot = 0>
struct LayoutDesc : DESCRIPTION
{
	LayoutDesc(const char* semantic);
};

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D10Blob;
struct ID3D11InputLayout;

class VertexShader
{
	ID3D11InputLayout* layout;
	ID3D11VertexShader* buffer;
public:
	VertexShader();
	VertexShader(const wchar_t*, const Array<DESCRIPTION>&);
	void Bind();
};

class PixelShader
{
	ID3D11PixelShader* buffer;
public:
	PixelShader();
	PixelShader(const wchar_t*);
	void Bind();
};


template<class T, uint stride>	struct layout_type;
template<> struct layout_type<float, 2> { inline static constexpr uint type = 16u; };
template<> struct layout_type<float, 3> { inline static constexpr uint type = 6u; };
template<> struct layout_type<float, 4> { inline static constexpr uint type = 2u; };
template<> struct layout_type<uint, 2>	{ inline static constexpr uint type = 17u; };
template<> struct layout_type<uint, 3>	{ inline static constexpr uint type = 7u; };
template<> struct layout_type<uint, 4>	{ inline static constexpr uint type = 3u; };
template<> struct layout_type<int, 2>	{ inline static constexpr uint type = 18u; };
template<> struct layout_type<int, 3>	{ inline static constexpr uint type = 8u; };
template<> struct layout_type<int, 4>	{ inline static constexpr uint type = 4u; };

template<uint stride, class type, uint slot>
inline LayoutDesc<stride, type, slot>::LayoutDesc(const char* semantic)
{
	SemanticName				= semantic;
	SemanticIndex				= 0;
	Format						= layout_type<type, stride>::type;
	InputSlot					= slot;
	AlignedByteOffset			= unsigned(-1);
	InputSlotClass				= 0;
	InstanceDataStepRate		= 0;
}
