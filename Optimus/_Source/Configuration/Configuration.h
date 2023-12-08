#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "..\..\External\libconfig++\libconfig.hpp"

namespace Optimus 
{
	namespace ConfigFunctions{

		bool loadConfig( const char* i_pNameConfig );
		void unloadConfig();

		libconfig::Config & getConfig();
	}
}

#endif