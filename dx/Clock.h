#pragma once

class Clock
{
	friend class Window;
	static void Update();
public:
	inline static float  dt;
	inline static double Time;
};
