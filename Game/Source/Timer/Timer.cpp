#include "GamePCH.h"
#include "Timer.h"

Timer::Timer(float duration, bool doesLoop)
{
	m_Elapsed = 0.0f;
	m_Duration = duration;
	m_IsRunning = false;
	m_DoesLoop = doesLoop;
}

Timer::Timer(const Timer& timer)
{
	m_Elapsed = timer.m_Elapsed;
	m_Duration = timer.m_Duration;
	m_IsRunning = timer.m_IsRunning;
	m_DoesLoop = timer.m_DoesLoop;
}

Timer::Timer()
{
	m_Elapsed = 0.0f;
	m_Duration = 0.0f;
	m_IsRunning = false;
	m_DoesLoop = false;
}

void Timer::Update(float delta)
{
	if (m_IsRunning == true)
	{
		m_Elapsed += delta;

		if (m_Elapsed >= m_Duration)
		{
			if (m_DoesLoop == true)
			{
				m_Elapsed = 0.0f;
			}
			else
			{
				m_IsRunning = false;
				m_Elapsed = m_Duration;
			}
		}
	}
}

void Timer::Start()
{
	m_IsRunning = true;
}

void Timer::Stop()
{
	m_IsRunning = false;
}

void Timer::Reset()
{
	m_Elapsed = 0.0f;
	m_IsRunning = false;
}

void Timer::SetDuration(float aDuration)
{
	m_Duration = aDuration;
}

void Timer::SetDoesLoop(bool aDoesLoop)
{
	m_DoesLoop = aDoesLoop;
}

float Timer::GetPercentage()
{
	return (float)(m_Elapsed / m_Duration);
}

double Timer::GetElapsed()
{
	return m_Elapsed;
}

float Timer::GetDuration()
{
	return m_Duration;
}

bool Timer::IsRunning()
{
	return m_IsRunning;
}

bool Timer::DoesLoop()
{
	return m_DoesLoop;
}