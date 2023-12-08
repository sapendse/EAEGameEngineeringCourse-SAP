#include "FlagController.h"

void FlagController::UpdateActor( Optimus::Actor& i_flag )
{
	if( m_isCaptured )
	{
		i_flag.m_gameObject->setWorldPosition( m_flagWorldPosition );
		m_isCaptured = false;
		if( m_reset )
		{
			i_flag.m_gameObject->setPosition( Optimus::v_Zero );
			m_reset = false;
		}
	}
	if( m_reset )
	{
		i_flag.m_gameObject->setPosition( Optimus::v_Zero );
		m_reset = false;
	}
}

void FlagController::SetWorldPosition( const Optimus::Vector3& i_playerWorldPosition )
{
	m_flagWorldPosition = i_playerWorldPosition;
}

void FlagController::SetFlagCaptured( bool i_flagState )
{
	m_isCaptured = i_flagState;
}

bool FlagController::GetCaptured()
{
	return m_isCaptured;
}

void FlagController::ResetFlag( bool i_reset )
{
	m_reset = i_reset;
}

void FlagCollider::OnCollision( Optimus::SharedPointer<Optimus::GameObject> & i_flag, Optimus::SharedPointer<Optimus::GameObject>& i_collidedTo, Optimus::Vector3& i_normal )
{
	if( "PlayerFlag" == i_flag->getTag() && "Player" == i_collidedTo->getTag() )
		Optimus::Messaging::getInstance()->UseMessage( "PFlagCaptured" );
	if( "EnemyFlag" == i_flag->getTag() && "AI" == i_collidedTo->getTag() )
		Optimus::Messaging::getInstance()->UseMessage( "EFlagCaptured" );
}