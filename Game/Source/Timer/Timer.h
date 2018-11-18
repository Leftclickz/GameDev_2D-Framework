#pragma once

class Timer
{

public:
	Timer(float duration, bool doesLoop = false);
	Timer(const Timer& timer);
	Timer();

	void Update(float deltatime);

	void Start();
	void Stop();
	void Reset();

	void SetDuration(float aDuration);
	void SetDoesLoop(bool aDoesLoop);

	float GetPercentage();

	double GetElapsed();
	float GetDuration();

	bool IsRunning();
	bool DoesLoop();

private:
	float m_Duration;
	float m_Elapsed;
	bool m_IsRunning;
	bool m_DoesLoop;
};