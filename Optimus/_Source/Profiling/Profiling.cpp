#include "Profiling.h"

Optimus::Profile::Profile()
{
	startTimer = false;
	QueryPerformanceFrequency( &perfFreq );
}

Optimus::Profile::~Profile()
{

}


void Optimus::Profile::start()
{
	if( !startTimer )
	{
		startTimer = true;
		LARGE_INTEGER temp;
		QueryPerformanceCounter( &temp );
		startTick = temp.QuadPart;
	}
}

float Optimus::Profile::end()
{
	if( startTimer )
	{
		LARGE_INTEGER temp;
		QueryPerformanceCounter( &temp );
		endTick = (LONGLONG)temp.QuadPart;
		double timeSpent = ( endTick - startTick ) * 1000 / double(perfFreq.QuadPart);
		startTick = endTick = 0;
		startTimer = false;
		return static_cast<float>(timeSpent);
	}
	return 0.0f;
}