#include "Configuration.h"
#include "..\Utilities\New.h"
#include <assert.h>

namespace Optimus
{
	namespace ConfigFunctions
	{
		libconfig::Config* m_pConfig = NULL;
	
		bool loadConfig( const char* i_pNameConfig )
		{
			assert( i_pNameConfig );

			m_pConfig = OPT_NEW libconfig::Config;
			assert( m_pConfig );
	
			if( m_pConfig->readFile( i_pNameConfig ) == false )
				return false;

			libconfig::Setting * pOptimusSettings = m_pConfig->lookup( "Optimus" );

			if( pOptimusSettings )
			{
				bool bRun = false;
				if( pOptimusSettings->lookupValue( "run", bRun ) )
					if( bRun )
						return true;
			}
			return false;
		}

		void unloadConfig()
		{
			if( m_pConfig )
			{
				delete( m_pConfig );
				m_pConfig = NULL;
			}
		}

		libconfig::Config & getConfig()
		{
			assert( m_pConfig != NULL );
	
			return *m_pConfig;
		}

	}
}