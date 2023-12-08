#include "Clock.h"

void Optimus::Clock::Initialize()
{
	s_cyclesPerSecond = (F32)ReadHiResTimerFrequency();
}

Optimus::Clock::Clock(F32 startTimeSeconds) :  
	m_timeCycles( secondsToCycles(startTimeSeconds)),  
	m_timeScale( 1.0f), // default to unscaled  
	m_isPaused( false) // default to running
{

}

Optimus::U64 Optimus::Clock::getTimeCycles() const
{
	return m_timeCycles;
}

Optimus::F32 Optimus::Clock::calcDeltaSeconds(const Clock& other) 
{  
	U64 dt = m_timeCycles - other.m_timeCycles;  
	return cyclesToSeconds(dt); 
}

void Optimus::Clock::Update(F32 dtRealSeconds) 
{  
	if (!m_isPaused)
	{      
		U64 dtScaledCycles = secondsToCycles(dtRealSeconds * m_timeScale);   
		m_timeCycles += dtScaledCycles; 
		m_deltaCycles = dtScaledCycles;
	}
	else
		m_deltaCycles = 0;
}

void Optimus::Clock::setPaused(bool isPaused) 
{  
	m_isPaused = isPaused; 
}

bool Optimus::Clock::isPaused() const 
{  
	return m_isPaused; 
}

void Optimus::Clock::setTimeScale(F32 scale) 
{  
	m_timeScale = scale; 
}

Optimus::F32 Optimus::Clock::getTimeScale() const 
{  
	return m_timeScale; 
}

Optimus::F32 Optimus::Clock::getDeltaTime() const
{
	return cyclesToSeconds(m_deltaCycles);
}
void Optimus::Clock::singleStep() 
{  
	if (m_isPaused)  
	{
		U64 dtScaledCycles = secondsToCycles(( 1.0f/30.0f) * m_timeScale);
		m_timeCycles += dtScaledCycles;  
	} 
}