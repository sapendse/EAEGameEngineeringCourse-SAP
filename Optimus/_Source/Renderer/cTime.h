/*
	This class manages all time-related functionality
*/

#ifndef _CTIME_H
#define _CTIME_H

// Header Files
//=============

#include "Initialization.h"

// Class Definition
//=================

namespace Optimus
{
	class cTime
	{
		// Interface
		//==========

	public:

		// Singleton Access
		//-----------------

		static inline cTime& GetSingleton()
		{
			static cTime singleton;
			return singleton;
		}
		#define g_time Optimus::cTime::GetSingleton()

		// Time Functions
		//---------------

		float GetTotalSecondsElapsed() const;
		float GetSecondsElapsedThisFrame() const;

		void OnNewFrame();

		// Initialization / Shutdown
		//--------------------------

		bool Initialize();

		// Data
		//=====

	private:

		LARGE_INTEGER m_countsPerSecond;
		double m_frequency;
		LARGE_INTEGER m_totalCountsElapsed_atInitializion;
		LARGE_INTEGER m_totalCountsElapsed_duringRun;
		LARGE_INTEGER m_totalCountsElapsed_previousFrame;

		// Implementation
		//===============

	private:

		// Initialization / Shutdown
		//--------------------------

		cTime();

	};
}

#endif	// _CTIME_H
