#ifndef _AI_CONTROLLER_H
#define _AI_CONTROLLER_H

#include "../Optimus/_Source/Loop/Optimus.h"
#include "PlayerController.h"
#include "FlagController.h"
#include "..\Optimus\_Source\Sounds\SoundManger.h"

enum EnemyState
{
	Capturing = 1,
	Returning,
	Chasing,
	Evading
};

class AIController : public Optimus::ActorControllerI
{
public:
	int						m_score, m_collisionRadius, m_framesToChaseOrEvade, m_index;
	bool					m_hasFlag, m_sprinting, m_destinationReached, m_returnedOnPath, m_offTrack;
	Optimus::Vector3		m_position, m_destination, m_direction;
	float					m_stamina;
	PlayerController*		m_playerController;
	FlagController*			m_enemyFlagController;
	EnemyState				m_currentState;
	Optimus::WayPointNode*	m_currentNode;
	Optimus::WayPointNode*	m_nextNode;
	FMOD::Channel*			m_hardChannel;
	FMOD::Channel*			m_softChannel;
	FMOD::Channel*			m_sprintChannel;
	FMOD::Channel*			m_currentChannel;

	AIController( PlayerController* i_playerController ) 
					:	m_score( 0 ),
						m_stamina( 100 ),
						m_playerController( i_playerController ),
						m_collisionRadius( 150 ),
						m_hasFlag( false ),
						m_sprinting( false ),
						m_destinationReached( false ),
						m_returnedOnPath( false ),
						m_framesToChaseOrEvade( 0 ),
						m_currentState( Capturing ),
						m_currentNode( NULL ),
						m_nextNode( NULL ),
						m_index( 0 ),
						m_offTrack( true )
	{

	}

	virtual void		UpdateActor( Optimus::Actor& i_AI );
	Optimus::Vector3	GetPosition();
	void				IncreaseScore();
	int					GetScore();
	float				GetStamina();
	bool				GetHasFlag();
	void				SetHasFlag( bool i_hasFlag );
	void				CheckState();
	void				MoveTowardsDestination( Optimus::Actor& i_AI );
	void				ChasePlayer( Optimus::Actor& i_AI );
	void				EvadePlayer( Optimus::Actor& i_AI );
	void				SetDestination( Optimus::Vector3& i_destination );
};

class AICollider : public Optimus::CollisionControllerI
{
public:
	virtual void		OnCollision( Optimus::SharedPointer<Optimus::GameObject> & i_AI, Optimus::SharedPointer<Optimus::GameObject>& i_collidedTo, Optimus::Vector3& i_normal );
	AICollider()	{}
	~AICollider()	{}

};
#endif