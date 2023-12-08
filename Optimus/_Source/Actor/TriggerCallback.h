#ifndef TRIGGER_CALLBACK_H
#define TRIGGER_CALLBACK_H

#include "GameObject.h"
#include "..\Helpers\SharedPointer.h"

namespace Optimus
{
	struct GameObject;

	class TriggerCallbackI
	{
	public:
		virtual void OnEnter( SharedPointer<GameObject> i_enter, SharedPointer<GameObject> i_trigger ) = 0;
		virtual void OnExit( SharedPointer<GameObject> i_enter, SharedPointer<GameObject> i_trigger ) = 0;
		virtual ~TriggerCallbackI() { }
	};
}

#endif