// Header Files
//=============

#include "cTime.h"

#include <cassert>

// Interface
//==========

// Time Functions
//---------------

float Optimus::cTime::GetTotalSecondsElapsed() const
{
	return static_cast<float>( static_cast<double>( m_totalCountsElapsed_duringRun.QuadPart ) * m_frequency );
}

float Optimus::cTime::GetSecondsElapsedThisFrame() const
{
	return static_cast<float>(
		static_cast<double>( m_totalCountsElapsed_duringRun.QuadPart - m_totalCountsElapsed_previousFrame.QuadPart )
		* m_frequency );
}

void Optimus::cTime::OnNewFrame()
{
	// Update the previous frame
	{
		m_totalCountsElapsed_previousFrame = m_totalCountsElapsed_duringRun;
	}
	// Update the current frame
	{
		LARGE_INTEGER totalCountsElapsed;
		BOOL result = QueryPerformanceCounter( &totalCountsElapsed );
		assert( result == TRUE );
		m_totalCountsElapsed_duringRun.QuadPart = totalCountsElapsed.QuadPart - m_totalCountsElapsed_atInitializion.QuadPart;
	}
}

// Initialization / Shutdown
//--------------------------

bool Optimus::cTime::Initialize()
{
	// Get the frequency of the high-resolution performance counter
	if ( QueryPerformanceFrequency( &m_countsPerSecond ) == TRUE )
	{
		m_frequency = 1.0 / static_cast<double>( m_countsPerSecond.QuadPart );
	}
	else
	{
		return false;
	}

	// Make sure it's supported on this hardware
	if ( m_countsPerSecond.QuadPart != 0 )
	{
		if ( QueryPerformanceCounter( &m_totalCountsElapsed_atInitializion ) == TRUE )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

// Implementation
//===============

// Initialization / Shutdown
//--------------------------

Optimus::cTime::cTime()
	:
	m_frequency( 0.0 )
{
	m_countsPerSecond.QuadPart = 0;
	m_totalCountsElapsed_atInitializion.QuadPart = 0;
	m_totalCountsElapsed_duringRun.QuadPart = 0;
	m_totalCountsElapsed_previousFrame.QuadPart = 0;
}
