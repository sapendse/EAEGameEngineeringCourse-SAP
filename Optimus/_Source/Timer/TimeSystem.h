#ifndef __TIMESYSTEM_H_
#define __TIMESYSTEM_H_

#include "Clock.h"
#include "..\Utilities\Target.h"
#include "..\Utilities\New.h"

namespace Optimus{

class TimeSystem
{	
	U64 endCycle;
	U64 beginCycle;
	Clock* GameTime; // may be a clock list in the future?
	
public:
	static F32 deltaTime;
	static TimeSystem* m_instance;

	TimeSystem() :
	endCycle(0),
	beginCycle(0),
	GameTime(NULL)
	{
	}

	static TimeSystem* getInstance()
	{
		if (m_instance ==  NULL)
			m_instance = OPT_NEW TimeSystem;

		return m_instance;
	}

	static inline F32 getDeltaTime() {return deltaTime;}
	F32 getPassedTime();
	bool Initialize();
	bool Update();
	bool ShutDown();	
};
}


#endif