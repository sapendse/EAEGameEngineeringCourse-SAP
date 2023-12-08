#ifndef __TIMER_H_
#define __TIMER_H_

#include "..\Utilities\Target.h"
#include <Windows.h>

namespace Optimus
{
class Clock
{
	U64 m_timeCycles;
	U64 m_deltaCycles;
	F32 m_timeScale;
	bool m_isPaused;
	

		static inline U64 secondsToCycles( F32 timeSeconds );
		
		static inline F32 ReadHiResTimerFrequency();
		

	public:
		static F32 s_cyclesPerSecond;

		static inline F32 cyclesToSeconds( U64 timeCycles ); //Only use to convert small durations into seconds. 
		static inline U64 GetCurrentCycleTime();

		static void Initialize();		// Call this when the game first starts up.

		explicit Clock(F32 startTimeSeconds = 0.0f);

		// This function should be called once per frame with the real measured frame time delta in seconds. 
		void Update( F32 dtRealSeconds );

		void singleStep();

		//get
		F32 getTimeScale() const;
		F32 getDeltaTime() const;
		bool isPaused() const;
		U64 getTimeCycles() const;


		//set
		void setPaused( bool isPaused );
		void setTimeScale( F32 scale );
		
		// Determine the difference between this clock’s absolute time and that of another clock, in seconds.
		F32 calcDeltaSeconds( const Clock& other );
		

};

}

#include "Clock.inl"

#endif