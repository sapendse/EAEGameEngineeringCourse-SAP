#include "TimeSystem.h"

bool Optimus::TimeSystem::Initialize()
{
	GameTime = OPT_NEW Clock();
	Clock::Initialize();
	beginCycle = Clock::GetCurrentCycleTime();
	deltaTime = 1.0f/30.0f;
	return true;
}

Optimus::F32 Optimus::TimeSystem::getPassedTime()
{
	U64 temp;
	temp = GameTime->getTimeCycles();
	return GameTime->cyclesToSeconds(temp);
}

bool Optimus::TimeSystem::Update()
{
	endCycle = Clock::GetCurrentCycleTime();
	deltaTime = Clock::cyclesToSeconds(endCycle - beginCycle);
	if (deltaTime > 1.0f/10.0f) 
	{  
		deltaTime = 1.0f/30.0f; 
	}
	GameTime->Update(deltaTime);
	deltaTime = GameTime->getDeltaTime();
	beginCycle = endCycle;
	return true;
}

bool Optimus::TimeSystem::ShutDown()
{
	delete GameTime;
	GameTime = 0;
	endCycle = 0;
	beginCycle = 0;
	delete m_instance;
	return true;
}
