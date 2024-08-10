#include "Timer.h"

Timer::Timer()
{
	start = std::chrono::high_resolution_clock::now();
}

void Timer::Reset()
{
	start = std::chrono::high_resolution_clock::now();
}

float Timer::GetDeltaT()
{
	auto now = std::chrono::high_resolution_clock::now();
	std::chrono::nanoseconds deltaT = now - start;
	return deltaT.count() / 1000000000.0f;
}
