#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include "../Optimus/_Source/Loop/Optimus.h"
#include "../Optimus/_Source/Sounds/SoundManger.h"

class PlayerController : public Optimus::ActorControllerI
{
	int					m_score;
	bool				m_hasFlag, m_soundPlayed;
	float				m_rotation;
	Optimus::F32		m_playerMoveSpeed, m_playerRotation, m_stamina;
	Optimus::Vector3	m_playerAcce, m_playerPosition;
	
public:
	FMOD::Channel*		m_hardChannel;
	FMOD::Channel*		m_softChannel;
	FMOD::Channel*		m_sprintChannel;
	FMOD::Channel*		m_currentChannel;

	PlayerController() :
						m_playerMoveSpeed		( 10 ),
						m_playerRotation		( 0 ),
						m_score					( 0 ),
						m_stamina				( 100 ),
						m_hasFlag				( false ),
						m_soundPlayed			( false )
						{}

	virtual void		UpdateActor( Optimus::Actor& i_player );
	bool				movePlayer ( Optimus::Actor& i_player );
	bool				rotatePlayer( Optimus::Actor& i_player );
	void				stopPlayer ( Optimus::Actor& i_player );
	void				IncreaseScore();
	int					GetScore();
	Optimus::F32		GetStamina();
	bool				GetHasFlag();
	void				SetHasFlag( bool i_hasFlag );
	float				GetRotation();
	Optimus::Vector3	getPosition();
};

class PlayerCollider : public Optimus::CollisionControllerI
{
public:
	virtual void		OnCollision( Optimus::SharedPointer<Optimus::GameObject> & i_player, Optimus::SharedPointer<Optimus::GameObject>& i_collidedTo, Optimus::Vector3& i_normal );
	PlayerCollider()	{}
	~PlayerCollider()	{}

};

#endif