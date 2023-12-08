#ifndef FLAG_MESSAGE_HANDLER_H
#define FLAG_MESSAGE_HANDLER_H

#include "..\Optimus\_Source\Loop\Optimus.h"
#include "FlagController.h"
#include "PlayerController.h"
#include "AIController.h"

class FlagMessageHandler : public Optimus::MessageCallbackI
{
	FlagController*		m_pFlagController;
	PlayerController*	m_playerController;
	FlagController*		m_eFlagController;
	AIController*		m_AIController;
	Optimus::Vector3	m_enemyWinArea, m_enemyFlagPos;
public:
	virtual void ProcessMessage( std::string i_message );
	FlagMessageHandler( FlagController* i_pFlagController, PlayerController* i_playerController, FlagController* i_eFlagController, AIController* i_AIController );
	void	SetEnemyWinArea( Optimus::Vector3& i_enemyWinAreaPos );
	void	SetEnemyFlagArea( Optimus::Vector3& i_enemyFlagAreaPos );
};

#endif