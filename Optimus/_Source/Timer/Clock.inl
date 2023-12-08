
inline Optimus::U64 Optimus::Clock::secondsToCycles(F32 timeSeconds)  
{  
	return (U64)(timeSeconds * s_cyclesPerSecond);
}
//Only use to convert small durations into seconds. 
inline Optimus::F32 Optimus::Clock::cyclesToSeconds(U64 timeCycles) 
{  
	return (F32)timeCycles / s_cyclesPerSecond; 
}

inline Optimus::F32 Optimus::Clock::ReadHiResTimerFrequency()
{
	LARGE_INTEGER i;
	QueryPerformanceFrequency(&i);
	return static_cast<F32>(i.QuadPart);
}

inline Optimus::U64 Optimus::Clock::GetCurrentCycleTime()
{
	LARGE_INTEGER i;
	QueryPerformanceCounter(&i);
	return i.QuadPart;
}

