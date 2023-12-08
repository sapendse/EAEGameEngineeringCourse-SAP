#ifndef TRIGGERS_H
#define TRIGGERS_H

#include "..\Actor\GameObject.h"
#include "..\Utilities\New.h"
#include "..\Actor\TriggerCallback.h"
#include <vector>

namespace Optimus{

	struct Trigger
	{
		TriggerCallbackI* m_triggerCallback;
		std::vector<GameObject*> m_gameObject;
	};

	class Triggers
	{
		public:
		static Triggers* m_instance;
		static Triggers* getInstance(){ 
			if( m_instance == 0 )
				m_instance = OPT_NEW Triggers;
			return m_instance;
		}
		void addTriggerCallback( TriggerCallbackI* i_triggerCallback );
		void removeTriggerCallback( TriggerCallbackI* i_triggerCallback );
	};

}


#endif