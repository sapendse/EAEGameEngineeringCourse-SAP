#include "GameEndStateTrigger.h"

GameEndStateTrigger::GameEndStateTrigger( GameEndStateTrigger& i_rhs )
{

}

GameEndStateTrigger::GameEndStateTrigger() :
TriggerBox()
{

}

void GameEndStateTrigger::OnEnter( Optimus::SharedPointer<Optimus::GameObject> i_gameObject, Optimus::SharedPointer<Optimus::GameObject> i_trigger )
{
	if( "PlayerWinArea" == i_trigger->getTag() && "Player" == i_gameObject->getTag() )
		Optimus::Messaging::getInstance()->UseMessage("PlayerWon");
	if( "EnemyWinArea" == i_trigger->getTag() && "AI" == i_gameObject->getTag() )
		Optimus::Messaging::getInstance()->UseMessage("EnemyWon");
}

void GameEndStateTrigger::OnExit( Optimus::SharedPointer<Optimus::GameObject> i_gameObject, Optimus::SharedPointer<Optimus::GameObject> i_trigger )
{

}