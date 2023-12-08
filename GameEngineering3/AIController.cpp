#include "AIController.h"

void AIController::SetDestination( Optimus::Vector3& i_destination )
{
	m_destination = i_destination;
	m_currentNode = NULL;
	m_nextNode = NULL;
	//m_direction = Optimus::v_Zero;
}

void AIController::IncreaseScore()
{
	m_score++;
}

int AIController::GetScore()
{
	return m_score;
}

float AIController::GetStamina()
{
	return m_stamina;
}

bool AIController::GetHasFlag()
{
	return m_hasFlag;
}

void AIController::SetHasFlag( bool i_hasFlag )
{
	m_hasFlag = i_hasFlag;
}

Optimus::Vector3 AIController::GetPosition()
{
	return m_position;
}

void AIController::ChasePlayer( Optimus::Actor& i_AI )
{
	if( m_framesToChaseOrEvade < 100 && m_playerController->GetHasFlag() )
	{
		Optimus::Vector3 _toPlayerPos = m_playerController->getPosition();
		_toPlayerPos -= m_position;
		if( _toPlayerPos.y() != 0.0f )
			_toPlayerPos.y(0.0f);
		_toPlayerPos = _toPlayerPos.normalise();
		_toPlayerPos = _toPlayerPos * i_AI.m_gameObject->GetSpeed();
		i_AI.m_gameObject->setPosition( i_AI.m_gameObject->getPosition() + _toPlayerPos * Optimus::TimeSystem::getDeltaTime() );
		m_framesToChaseOrEvade++;
	}
	else
	{
		Optimus::WayPointNode* _closestNode = Optimus::WayPointGraph::GetInstance()->GetClosest( m_position );
		if( _closestNode != NULL ) //If false; there are no way points; fix it.
		{
			Optimus::Vector3 _toNodePos = m_position - _closestNode->GetPosition();
			float _dist = sqrt( Optimus::Vector3::magSqr( _toNodePos ) );
			if( _dist < 55.0f )
			{
				i_AI.m_gameObject->setPosition( _closestNode->GetPosition() );
				m_returnedOnPath = true;
				m_framesToChaseOrEvade = 0;
			}
			if( _toNodePos.y() != 0.0f )
				_toNodePos.y(0.0f);
			_toNodePos = _toNodePos.normalise();
			_toNodePos = _toNodePos * i_AI.m_gameObject->GetSpeed();
			i_AI.m_gameObject->setPosition( i_AI.m_gameObject->getPosition() + _toNodePos * Optimus::TimeSystem::getDeltaTime() );
		}
	}
}

void AIController::EvadePlayer( Optimus::Actor& i_AI )
{
	if( m_framesToChaseOrEvade < 100 )
	{
		Optimus::Vector3 _fromPlayerPos = m_playerController->getPosition();
		_fromPlayerPos = m_position - _fromPlayerPos;
		if( _fromPlayerPos.y() != 0.0f )
			_fromPlayerPos.y(0.0f);
		_fromPlayerPos = _fromPlayerPos.normalise();
		_fromPlayerPos = _fromPlayerPos * i_AI.m_gameObject->GetSpeed();
		i_AI.m_gameObject->setPosition( i_AI.m_gameObject->getPosition() + _fromPlayerPos * Optimus::TimeSystem::getDeltaTime() );
		m_framesToChaseOrEvade++;
	}
	else
	{
		Optimus::WayPointNode* _closestNode = Optimus::WayPointGraph::GetInstance()->GetClosest( m_position );
		if( _closestNode != NULL ) //If false; there are no way points; fix it.
		{
			Optimus::Vector3 _toNodePos = m_position - _closestNode->GetPosition();
			float _dist = sqrt( Optimus::Vector3::magSqr( _toNodePos ) );
			if( _dist < 55.0f )
			{
				i_AI.m_gameObject->setPosition( _closestNode->GetPosition() );
				m_returnedOnPath = true;
				m_framesToChaseOrEvade = 0;
			}
			if( _toNodePos.y() != 0.0f )
				_toNodePos.y(0.0f);
			_toNodePos = _toNodePos.normalise();
			_toNodePos = _toNodePos * i_AI.m_gameObject->GetSpeed();
			i_AI.m_gameObject->setPosition( i_AI.m_gameObject->getPosition() + _toNodePos * Optimus::TimeSystem::getDeltaTime() );
		}
	}
}

void AIController::CheckState()
{
	Optimus::Vector3 _toPlayerPos = m_playerController->getPosition();
	_toPlayerPos -= m_position;
	m_index = 0;
	if( (int)sqrt( Optimus::Vector3::magSqr( _toPlayerPos ) ) > m_collisionRadius )
	{
		if( m_hasFlag )
			m_currentState = Returning;
		else
			m_currentState = Capturing;
		m_destinationReached = false;
		Optimus::WayPointGraph::GetInstance()->CalculatePath( m_position, m_destination, D3DCOLOR_XRGB( 255, 0, 0 ) );
	}
	else
	{
		if( m_playerController->GetHasFlag() )
		{
			if( m_hasFlag )
			{
				if( !m_returnedOnPath )
					m_currentState = Evading;
			}
			else
			{
				if( !m_returnedOnPath )
					m_currentState = Chasing;
			}
		}
	}
}

void AIController::UpdateActor( Optimus::Actor& i_AI )
{
	if( m_currentChannel != NULL )
		Optimus::SoundManager::getInstance()->PauseChannel(m_currentChannel, true);
	if( m_sprintChannel != NULL )
		Optimus::SoundManager::getInstance()->PauseChannel(m_sprintChannel, true);

	if( i_AI.m_gameObject->getSurface() )
		m_currentChannel = m_hardChannel; // Play Music On Hard Surface
	else
		m_currentChannel = m_softChannel;
	CheckState();
	i_AI.m_gameObject->SetSpeed( 75.0f );
	if( m_stamina == 100.0f && !m_hasFlag )
	{
		m_sprinting = true;
	}
	if( m_sprinting )
	{
		if( m_stamina > 0.0f )
		{
			m_stamina -= 1.0f;
			Optimus::SoundManager::getInstance()->PauseChannel( m_sprintChannel, false );
			i_AI.m_gameObject->SetSpeed( 150.0f );
		}
		else
		{
			m_stamina = 0.0f;
			m_sprinting = false;
		}
	}
	else
	{
		if( m_stamina < 100.0f )
			m_stamina += 0.5f;
		else
			m_stamina = 100.0f;
	}

	switch( m_currentState )
	{
		case Capturing:
		{
			if( !m_destinationReached )
			{
				MoveTowardsDestination( i_AI );
			}
			break;
		}
		case Returning:
		{
			if( !m_destinationReached )
			{
				MoveTowardsDestination( i_AI );
			}
			break;
		}
		case Chasing:
		{
			ChasePlayer(i_AI);
			break;
		}
		case Evading:
		{
			EvadePlayer(i_AI);
			break;
		}
	}
	m_position = i_AI.m_gameObject->getNextPos();
}

void AIController::MoveTowardsDestination( Optimus::Actor& i_AI )
{
	std::vector<Optimus::WayPointNode*> pathNodes = Optimus::WayPointGraph::GetInstance()->m_currentPath;
	if( pathNodes.size() == 0 ) //No path but through.
	{
		Optimus::Vector3 _towardsDest = m_destination - m_position;
		float dist = sqrt( Optimus::Vector3::magSqr( _towardsDest ) );
		if( dist < 200.0f )
		{
			m_direction = _towardsDest.normalise();
		}
		if( dist < 80.0f )
		{
			i_AI.m_gameObject->setPosition( Optimus::WayPointGraph::GetInstance()->m_destinationNode->GetPosition() );
			i_AI.m_gameObject->setVelocity( Optimus::v_Zero );
		}
	}
	else		//Follow the yellow brick road.
	{
		if( m_currentNode == NULL && m_nextNode == NULL )
		{
			m_currentNode = pathNodes[0];
			m_nextNode = pathNodes[1];
			m_direction = m_nextNode->GetPosition() - m_currentNode->GetPosition();
			m_direction = m_direction.normalise();
		}

		if( m_currentNode->GetID() == Optimus::WayPointGraph::GetInstance()->m_destinationNode->GetID() )
		{
			m_destinationReached = true;
			Optimus::Vector3 _towardsDest = i_AI.m_gameObject->getPosition() - m_currentNode->GetPosition();
			float dist = sqrt( Optimus::Vector3::magSqr( _towardsDest ) );
			if( dist < 80.0f )
			{
				i_AI.m_gameObject->setPosition( m_currentNode->GetPosition() );
				i_AI.m_gameObject->setVelocity( Optimus::v_Zero );
				return;
			}
		}

		Optimus::Vector3 _towardsDest = i_AI.m_gameObject->getPosition() - m_nextNode->GetPosition();
		float dist = sqrt( Optimus::Vector3::magSqr( _towardsDest ) );
		if( m_nextNode != NULL && ( dist < 80.0f ) )
		{
			m_currentNode = NULL;
			m_nextNode = NULL;
		}
		else
		{
			m_direction = m_nextNode->GetPosition() - m_currentNode->GetPosition();
			m_direction = m_direction.normalise();
			m_direction *= i_AI.m_gameObject->GetSpeed();
			i_AI.m_gameObject->setVelocity( m_direction );
		}
	}
}

void AICollider::OnCollision( Optimus::SharedPointer<Optimus::GameObject> & i_AI, Optimus::SharedPointer<Optimus::GameObject>& i_collidedTo, Optimus::Vector3& i_normal )
{
	//If AI collides with Player; send a message.
	if( "Player" == i_collidedTo->getTag() )
	{
		Optimus::Messaging::getInstance()->UseMessage( "EnemyTaggedPlayer" );
	}
}