#ifndef WIN32_ENGINE_TIMER_H
#define WIN32_ENGINE_TIMER_H

#include <windows.h>
#include "./EngineTimer.h"

class Win32EngineTimer : public EngineTimer
{
public:
	Win32EngineTimer();
	virtual ~Win32EngineTimer();

public:
	float totalTime();
	float deltaTime();

	void reset();
	void start();
	void stop();
	void tick();
	float getFPS();

private:
	double m_dSecondsPerCount;
	double m_dDeltaTime;

	__int64 m_i64BaseTime;
	__int64 m_i64PauseTime;
	__int64 m_i64StopTime;
	__int64 m_i64PrevTime;
	__int64 m_i64CurrTime;

	bool m_bStop;
};

#endif