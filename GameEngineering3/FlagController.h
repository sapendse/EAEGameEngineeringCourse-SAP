#ifndef FLAG_CONTROLLER_H
#define FLAG_CONTROLLER_H

#include "..\Optimus\_Source\Loop\Optimus.h"

class FlagController : public Optimus::ActorControllerI
{
	public:
	bool				m_isCaptured, m_reset;
	Optimus::Vector3	m_flagWorldPosition;

	FlagController() :
	  m_isCaptured( false ),
	  m_reset( false ),
	  m_flagWorldPosition( Optimus::v_Zero )
	  {}

	virtual void	UpdateActor( Optimus::Actor& i_flag );
	void			SetWorldPosition( const Optimus::Vector3& i_playerWorldPosition );
	void			SetFlagCaptured( bool i_flagState );
	void			ResetFlag( bool i_reset );
	bool			GetCaptured();
};

class FlagCollider : public Optimus::CollisionControllerI
{
	public:
	virtual void		OnCollision( Optimus::SharedPointer<Optimus::GameObject> & i_flag, Optimus::SharedPointer<Optimus::GameObject>& i_collidedTo, Optimus::Vector3& i_normal );
	FlagCollider()	{}
	~FlagCollider()	{}
};

#endif