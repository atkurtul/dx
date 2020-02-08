#pragma once
#include "Font.h"
#include "Window.h"
#include "std/list.h"

class UI
{
public:
	struct TEXT
	{
		String	text;
		vec2	pos;
		float	scale = 1.f;
	};

	inline static List<TEXT> texts;
	inline static Font font;
	inline static VertexShader vs;
	inline static PixelShader ps;
	static void init(const char* font, const char* vsp, const char* psp);
	static void Render(const String& text, float x, float y, float scale = 1.f);
	static void RenderAll();
};

struct Text
{
	inline static uint global_id = 0;
	uint id;
	Text() : id(-1)
	{
	}
	void check()
	{
		if (id != -1) return;
		id = global_id++;
		UI::texts.push({});
	}
	Text& operator = (const vec2& v)
	{
		check();
		UI::texts[id].pos = v;
		return *this;
	}
	Text& operator ++()
	{
		check();
		UI::texts[id].scale *= 1.25f;
		return *this;
	}
	Text& operator = (const String& s)
	{
		check();
		UI::texts[id].text = s;
		return *this;
	}
	Text& operator = (String&& s)
	{
		check();
		UI::texts[id].text = XMOVE(s);
		return *this;
	}
};

class GUI
{
public:
	inline static VertexShader	vs;
	inline static PixelShader	ps;
	inline static Texture		Default;
	static void init(const char* vsp, const char* psp);

	struct BUTTON
	{
		inline static Texture		Tex;
		inline static VertexArray	Quad;
		Text		text;
		vec3		Color = 1.f;
		vec4		xf = { 0, 0, 120.f, 60.f };
		char		state = 0;

		BUTTON()
		{
			if (Quad.id != -1) return;
			Tex = "button.png";
			Quad.Buffer(Array<vec4>
			{
				{ 0, 0, 0, 0 },
				{ 0, 1,		0, 1 },
				{ 1, 0,		1, 0 },
				{ 1, 1,		1, 1 }
			});
		}

		void Draw()
		{
			Uniform(xf, 2);
			if (state) Uniform(Color * 2.f, 3);
			else Uniform(Color, 3);
			Quad.Draw();
		}
	};

	inline static Array<BUTTON> buttons;
	static void Update()
	{
		BUTTON::Tex.Bind(0);
		vs.Bind();
		ps.Bind();
		Graphics::SetTopology(5);
		Uniform(Window::InvSize, 1);
		bool updateflag = 0;
		for (auto& btn : buttons)
		{
			btn.Draw();	
			if (btn.state == 1)btn.state = 0;
			if (updateflag) continue;
			bool left = Input::Button(0);
			if (btn.state > 1 && left)
			{
				btn.xf.xy = Mouse::Pos - btn.xf.zw * .5f;
				btn.text = btn.xf.xy;
				updateflag = 1;
				continue;
			}
			if (Mouse::CheckPos(btn.xf))
			{
				if (Mouse::CheckBtn(btn.xf, 0))
				{
					btn.state = 2;
					updateflag = 1;
					continue;
				}
				else if (! left)	btn.state = 1;
			}
			else btn.state = 0;
		}
		Graphics::SetTopology(4);
	}
};


struct Button
{
	inline static uint global_id = 0;
	uint id;
	Button() : id(-1)
	{

	}
	void check()
	{
		if (id != -1) return;
		id = global_id++;
		GUI::buttons.push({});
	}
	Button& operator = (const vec2& v)
	{
		check();
		GUI::buttons[id].xf.xy = v;
		GUI::buttons[id].text = v;
		return *this;
	}
	Button& operator ++()
	{
		check();
		GUI::buttons[id].xf.zw *= 1.25f;
		++GUI::buttons[id].text;
		return *this;
	}
	Button& operator = (const String& s)
	{
		check();
		GUI::buttons[id].text = s;
		return *this;
	}
	Button& operator = (String&& s)
	{
		check();
		GUI::buttons[id].text = XMOVE(s);
		return *this;
	}

};