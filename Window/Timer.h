#pragma once

class Timer
{
public:
	Timer();
	float TotalTime() const;
	float DeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:
	double cSegundosPorCount;
	double cDeltaTime;

	__int64 cBaseTime;
	__int64 cPausedTime;
	__int64 cStopTime;
	__int64 cPrevTime;
	__int64 cCurrTime;

	bool cPausado;
};