#include "Timer.h"
#include <Windows.h>

Timer::Timer()
	:
	cSegundosPorCount(0.0), cDeltaTime(-1.0), cBaseTime(0),
	cPausedTime(0), cPrevTime(0), cCurrTime(0), cPausado(false)
{
	__int64 countsPorSeg = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPorSeg);
	cSegundosPorCount = 1.0 / (double)countsPorSeg;
}

void Timer::Tick()
{
	if (cPausado)
	{
		cDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	cCurrTime = currTime;

	cDeltaTime = (cCurrTime - cPrevTime) * cSegundosPorCount;

	cPrevTime = cCurrTime;

	if (cDeltaTime < 0.0)
	{
		cDeltaTime = 0.0;
	}
}

float Timer::DeltaTime() const
{
	return (float)cDeltaTime;
}

void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	cBaseTime = currTime;
	cPrevTime = currTime;
	cStopTime = 0;
	cPausado = 0;
}

void Timer::Stop()
{
	if (!cPausado)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		cStopTime = currTime;
		cPausado = true;
	}
}

void Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (cPausado)
	{
		cPausedTime += (startTime - cStopTime);

		cPrevTime = startTime;

		cStopTime = 0;

		cPausado = false;
	}
}

float Timer::TotalTime() const
{
	if (cPausado)
	{
		return (float)(((cStopTime - cPausedTime) - cBaseTime) * cSegundosPorCount);
	}
	else
	{
		return (float)(((cCurrTime - cPausedTime) - cBaseTime) * cSegundosPorCount);
	}
}