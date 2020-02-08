#pragma once
#include "VertexArray.h"
#include "Texture.h"
#include "Shader.h"
#include "std/string.h"
class Font
{
public:
	friend class UI;
	struct Char
	{
		float advance;
		VertexArray quad;
	};
	float line;
	Texture Atlas;
	Char charmap[128];
public:
	Font();
	Font(const char* path);
};
