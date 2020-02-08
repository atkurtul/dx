#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Window.h"
#include "Graphics.h"
#include "Shader.h"
#include "VertexArray.h"
#include "Cam.h"
#include "AnimatedModel.h"
#include "Texture.h"
#include "GUI.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)					
{																				
	Window::Init(instance, 1600, 900);											
	Graphics::Init();															
	Graphics::InitDepthBuffer();												
	Graphics::ClearColor(0.3f, 0.4f, 1.f, 1.f);									
	Array<DESCRIPTION> desc =													
	{																			
		LayoutDesc<3, float>		{"POSITION"},								
		LayoutDesc<3, float>		{"COLOR"},									
		LayoutDesc<2, float>		{"TEXCOORD"},							
		LayoutDesc<4, uint>			{"JOINT"},									
		LayoutDesc<4, float>		{"WEIGHT"},									
	};																			
	VertexShader vs(DEBUG_VS, desc);
	PixelShader  ps(DEBUG_PS);
	mat4 id;
	Cam cam;
	Uniform(cam.prj, 0);
	Uniform(id, 2);
	//Model model("run.dae");
	Model model("thrill.dae");
	model.xform = mat4(0.01f) * axang(1, 0, 0, -1.57);
	Texture t("tauren.png");
	//model.xform = mat4(100.f);
	UI::init("consolas.fnt", "..\\x64\\Debug\\UIvs.cso", "..\\x64\\Debug\\UIps.cso");
	GUI::init("..\\x64\\Debug\\GUIvs.cso", "..\\x64\\Debug\\GUIps.cso");
	Array<Button> buttons;
	for (int i = 0; i < 10; ++i)
	{
		buttons.push({});
		buttons.back() = vec2(i * 50, i * 50);
	}

	while (Window::PollEvents())												
	{			
		vs.Bind();
		ps.Bind();
		Graphics::Clear();																	
		cam.update();
		Uniform(cam.view, 1);
		static float speed = 1.f;
		if (Input::Key('1')) speed -= Clock::dt;
		if (Input::Key('2')) speed += Clock::dt;
		if (Input::Button(1)) Mouse::Hide();
		else Mouse::Show();
		static float q = 0;
		model.Rotate(mat4(axang(1, 1, 1, Clock::dt)));
		t.Bind(0);
		model.Draw();	
		buttons[0] = tostr(1.f / Clock::dt);
		buttons[1] = tostr(Mouse::Click[0]);
		buttons[2] = tostr(Mouse::Normalized);
		buttons[3] = tostr(Input::Button(0));
		UI::RenderAll();
		GUI::Update();
		Graphics::SwapBuffers(0);
	}																			
}																				






