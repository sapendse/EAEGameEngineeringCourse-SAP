#include "PlayerController.h"

void PlayerController::UpdateActor( Optimus::Actor& i_player )
{
	if( m_currentChannel != NULL )
		Optimus::SoundManager::getInstance()->PauseChannel(m_currentChannel, true);
	if( m_sprintChannel != NULL )
		Optimus::SoundManager::getInstance()->PauseChannel(m_sprintChannel, true);

	if( i_player.m_gameObject->getSurface() )
		m_currentChannel = m_hardChannel; // Play Music On Hard Surface
	else
		m_currentChannel = m_softChannel;
	if( !Optimus::cTPSCamera::getInstance()->m_freeFormCamera )
	{
		if( !movePlayer( i_player ) && i_player.m_gameObject->getGrounded() )
		{
			stopPlayer( i_player );
		}
	
		if( !rotatePlayer( i_player ) )
		{
			i_player.setHasRotated( false );
		}
	}
	else
	{
		stopPlayer( i_player );
	}
	m_playerPosition = i_player.m_gameObject->getNextPos();
	m_rotation = i_player.m_gameObject->getRotation();
	if( Optimus::UserInput::IsKeyPressed( 'W' ) || Optimus::UserInput::IsKeyPressed( 'S' ) )
	{
		Optimus::SoundManager::getInstance()->PauseChannel( m_currentChannel, false );
	}
}

bool PlayerController::GetHasFlag()
{
	return m_hasFlag;
}

float PlayerController::GetRotation() 
{
	return m_rotation;
}

void PlayerController::SetHasFlag( bool i_hasFlag )
{
	m_hasFlag = i_hasFlag;
	if( m_soundPlayed == false )
	{
		Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/youhavetheflag.wav" );
		m_soundPlayed = true;
	}
}

bool PlayerController::movePlayer( Optimus::Actor& i_player )
{
	m_playerAcce = Optimus::v_Zero;
	Optimus::Vector3 forward;
	Optimus::Vector3::convertToVector3( Optimus::cTPSCamera::getInstance()->getForward(), forward );
	i_player.m_gameObject->SetSpeed( 100.0f );
	if( Optimus::UserInput::IsKeyPressed( VK_SHIFT ) )
	{
		if( m_stamina > 0.0f )
		{
			m_stamina -= 1.0f;
			Optimus::SoundManager::getInstance()->PauseChannel( m_sprintChannel, false );
			i_player.m_gameObject->SetSpeed( 200.0f );
		}
		else
			m_stamina = 0.0f;
	}
	else
	{
		if( m_stamina < 100.0f )
			m_stamina += 0.5f;
		else
			m_stamina = 100.0f;
	}
	if( Optimus::UserInput::IsKeyPressed( 'W' ) )
	{
		m_playerAcce += forward;
		i_player.m_gameObject->setVelocity( m_playerAcce * i_player.m_gameObject->GetSpeed() );
		return true;
	}
	if( Optimus::UserInput::IsKeyPressed( 'S' ) )
	{
		m_playerAcce -= forward;
		i_player.m_gameObject->setVelocity( m_playerAcce * i_player.m_gameObject->GetSpeed() );
		return true;
	}
	return false;
}

bool PlayerController::rotatePlayer( Optimus::Actor& i_player )
{
	if( Optimus::UserInput::IsKeyPressed( 'A' ) )
	{
		m_playerRotation += 0.04f;
		i_player.m_gameObject->setRotation( m_playerRotation );
		i_player.setHasRotated( true );
		return true;
	}
	if( Optimus::UserInput::IsKeyPressed( 'D' ) )
	{
		m_playerRotation -= 0.01f;
		i_player.m_gameObject->setRotation( m_playerRotation );
		i_player.setHasRotated( true );
		return true;
	}
	return false;
}

Optimus::Vector3 PlayerController::getPosition()
{
	return m_playerPosition;
}

void PlayerController::stopPlayer( Optimus::Actor& i_player )
{
	i_player.m_gameObject->setAcceleration( Optimus::v_Zero );
	i_player.m_gameObject->setVelocity( Optimus::v_Zero );
}

void PlayerController::IncreaseScore()
{
	m_score++;
}

int PlayerController::GetScore()
{
	return m_score;
}

float PlayerController::GetStamina()
{
	return m_stamina;
}

void PlayerCollider::OnCollision( Optimus::SharedPointer<Optimus::GameObject> & i_player, Optimus::SharedPointer<Optimus::GameObject>& i_collidedTo, Optimus::Vector3& i_normal )
{
	//If Player tagged flag; send a message.
	if( "AI" == i_collidedTo->getTag() )
	{
		Optimus::Messaging::getInstance()->UseMessage( "PlayerTaggedEnemy" );
	}

	//Do nothing when the Player collides with flag.
}