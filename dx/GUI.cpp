#include "GUI.h"
#include "Logger.h"
#include "Window.h"

void UI::init(const char* fpath, const char* vsp, const char* psp)
{
	font = Font(fpath);
	vs = VertexShader(wstr(vsp), { LayoutDesc<2, float>("POSITION"), LayoutDesc<2, float>("TEXCOORD") });
	ps = PixelShader(wstr(psp));
}

void UI::Render(const String& text, float x, float y, float scale)
{
	vs.Bind();
	ps.Bind();
	font.Atlas.Bind(0);
	float xx = x;
	Uniform(Window::InvSize, 1);
	for (char c : text)
	{
		Font::Char& C = font.charmap[c];
		Uniform(vec3(xx, y, scale), 2);
		C.quad.Draw();
		xx += C.advance * scale;
	}
}

void UI::RenderAll()
{
	vs.Bind();
	ps.Bind();
	font.Atlas.Bind(0);
	Uniform(Window::InvSize, 1);
	for (auto& text : texts)
	{
		float xx = text.pos.x;
		for (char c : text.text)
		{
			Font::Char& C = font.charmap[c];
			Uniform(vec3(xx, text.pos.y, text.scale), 2);
			C.quad.Draw();
			xx += C.advance * text.scale;
		}
	}
}

void GUI::init(const char* vsp, const char* psp)
{
	vs = VertexShader(wstr(vsp), { LayoutDesc<2, float>("POSITION"), LayoutDesc<2, float>("TEXCOORD") });
	ps = PixelShader(wstr(psp));
	Default = "default.jpg";
}
