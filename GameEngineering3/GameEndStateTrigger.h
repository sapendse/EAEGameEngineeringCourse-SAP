#ifndef GAME_END_STATE_TRIGGER_H
#define GAME_END_STATE_TRIGGER_H

#include "..\Optimus\_Source\Loop\Optimus.h"

class GameEndStateTrigger : public Optimus::TriggerBox
{
public:
	GameEndStateTrigger();
	GameEndStateTrigger( GameEndStateTrigger& i_rhs );
	virtual void OnEnter( Optimus::SharedPointer<Optimus::GameObject> i_gameObject, Optimus::SharedPointer<Optimus::GameObject> i_trigger );
	virtual void OnExit( Optimus::SharedPointer<Optimus::GameObject> i_gameObject, Optimus::SharedPointer<Optimus::GameObject> i_trigger );
};

#endif