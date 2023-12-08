#ifndef PROFILING_H
#define PROFILING_H

#include <Windows.h>

namespace Optimus{

	class Profile
	{
		LONGLONG startTick;
		LONGLONG endTick;
		LARGE_INTEGER perfFreq;
		bool startTimer;

	public:
		//Constructor and destructor
		Profile();
		~Profile();

		//Functions
		void start();
		float end();
	};

}

#endif