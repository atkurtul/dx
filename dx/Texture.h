#pragma once
#include "Graphics.h"

struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;
class Texture
{
	ID3D11ShaderResourceView* view;
	ID3D11Texture2D* texture;
public:
	float width, height;
	Texture();
	Texture(const char* path);
	void Bind(unsigned);
};

