#include "Win32EngineTimer.h"

Win32EngineTimer::Win32EngineTimer():
	m_dSecondsPerCount(0.0),
	m_dDeltaTime(-1.0),
	m_i64BaseTime(0),
	m_i64PauseTime(0),
	m_i64PrevTime(0),
	m_i64CurrTime(0),
	m_bStop(false)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_dSecondsPerCount = 1.0 / (double)countsPerSec;
}

Win32EngineTimer::~Win32EngineTimer()
{

}

float Win32EngineTimer::totalTime()
{
	if (m_bStop)
	{
		return (float)(((m_i64StopTime - m_i64PauseTime) - m_i64BaseTime) * m_dSecondsPerCount);
	}
	else
	{
		return (float)(((m_i64CurrTime - m_i64PauseTime) - m_i64BaseTime) * m_dSecondsPerCount);
	}
}

float Win32EngineTimer::deltaTime()
{
	return (float)m_dDeltaTime;
}

void Win32EngineTimer::reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_i64BaseTime = currTime;
	m_i64PrevTime = currTime;
	m_i64StopTime = 0;
	m_bStop = false;
}

void Win32EngineTimer::start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_bStop)
	{
		m_i64PauseTime += (startTime - m_i64StopTime);

		m_i64PrevTime = startTime;
		m_i64StopTime = 0;
		m_bStop = false;
	}
}

void Win32EngineTimer::stop()
{
	if (!m_bStop)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_i64StopTime = currTime;
		m_bStop = true;
	}
}

void Win32EngineTimer::tick()
{
	if (m_bStop)
	{
		m_dDeltaTime = 0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_i64CurrTime = currTime;

	m_dDeltaTime = (m_i64CurrTime - m_i64PrevTime) * m_dSecondsPerCount;

	m_i64PrevTime = m_i64CurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (m_dDeltaTime < 0.0)
	{
		m_dDeltaTime = 0.0;
	}
}

float Win32EngineTimer::getFPS()
{
	return (float)(1.0 / m_dDeltaTime);
}