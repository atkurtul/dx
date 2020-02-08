#pragma once
#include "Window.h"
#include "std/quaternion.h"

class Cam
{
	vec3 pos;
	vec3& left;
	vec3& up;
	vec3& dir;
public:
	mat4 prj;
	mat4 view;
	Cam() :
		prj(perspective(85.f, Window::Size.y / Window::Size.x, 0.01f, 4000.f)),
		left((vec3&)view.row[0]),
		up((vec3&)view.row[1]),
		dir((vec3&)view.row[2])
	{
	}
	char Forward = 'W';
	char Backward = 'S';
	char Left = 'A';
	char Right = 'D';
	char Up = 'R';
	char Down = 'F';
	void move()
	{
		if (Input::Key(Forward))	pos += view.row[2].xyz * Clock::dt * 10.f;
		if (Input::Key(Backward))	pos -= view.row[2].xyz * Clock::dt * 10.f;
		if (Input::Key(Right))		pos += view.row[0].xyz * Clock::dt * 10.f;
		if (Input::Key(Left))		pos -= view.row[0].xyz * Clock::dt * 10.f;
		if (Input::Key(Up))			pos += view.row[1].xyz * Clock::dt * 10.f;
		if (Input::Key(Down))		pos -= view.row[1].xyz * Clock::dt * 10.f;
	}
	void update()
	{
		move();
		mat3 m = roty(Clock::dt * 0.5f * Mouse::Delta.x);
		left = m * left;
		m = mat3(axang(left, -Clock::dt * 0.5f * Mouse::Delta.y)) * m;
		up = m * up;
		dir = m * dir;
		view.col[3].xyz = view * -pos;
	}
};

