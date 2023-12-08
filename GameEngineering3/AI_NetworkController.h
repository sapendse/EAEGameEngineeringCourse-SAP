#ifndef _AI_NETWORK_CONTROLLER_H
#define _AI_NETWORK_CONTROLLER_H

#include "../Optimus/_Source/Loop/Optimus.h"
#include "PlayerController.h"
#include "FlagController.h"
#include "..\Optimus\_Source\Sounds\SoundManger.h"

class AI_NetworkController : public Optimus::ActorControllerI
{
public:
	Optimus::Vector3 m_position;
	float m_stamina, m_rotation;
	int	  m_score;
	bool  m_hasFlag, m_soundPlayed;
	PlayerController* m_playerController;

	AI_NetworkController( PlayerController* i_controller ) :
		m_stamina( 100.0f ),
		m_rotation( 0.0f ),
		m_score( 0 ),
		m_hasFlag( false ),
		m_soundPlayed( false )
	{
		m_playerController = i_controller;
	}

	virtual void		UpdateActor( Optimus::Actor& i_AI );
	void				SetHasFlag( bool i_hasFlag );
};

#endif