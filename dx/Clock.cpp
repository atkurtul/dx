#include "Clock.h"
#include <chrono>

void Clock::Update()
{
	static 	std::chrono::high_resolution_clock	mainclock;
	Time = static_cast<double>(mainclock.now().time_since_epoch().count()) * 1e-9;
	static double prev = Time;
	dt = float(Time - prev);
	prev = Time;
}