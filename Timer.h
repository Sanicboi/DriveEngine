#pragma once
#include "Main.h"


class Timer
{
private:
	std::chrono::steady_clock::time_point start;
public:
	Timer();
	void Reset();
	float GetDeltaT();
};

