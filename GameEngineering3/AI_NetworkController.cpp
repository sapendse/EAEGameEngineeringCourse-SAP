#include "AI_NetworkController.h"
#include "..\Optimus\_Source\Networking\NetworkManager.h"

void AI_NetworkController::UpdateActor( Optimus::Actor& i_AI )
{
	m_position = m_playerController->getPosition();
	
	Optimus::GameObject_Minimal currObject;
	currObject._x = m_position.x();
	currObject._y = m_position.y();
	currObject._z = m_position.z();
	currObject.m_rotation = m_rotation;
	Optimus::NetworkManager::getInstance()->SendGameObject( currObject );
	i_AI.m_gameObject->setRotation( m_rotation );

	//Optimus::PlayerInfo currInfo;
	//currInfo.m_hasFlag = Optimus::NetworkManager::getInstance()->m_playerInfo.m_hasFlag;
	//currInfo.m_stamina = Optimus::NetworkManager::getInstance()->m_playerInfo.m_stamina;
	//currInfo.m_score = Optimus::NetworkManager::getInstance()->m_playerInfo.m_score;

	Optimus::Vector3 pos;
	pos.x( Optimus::NetworkManager::getInstance()->m_gameObjectInfo._x );
	pos.y( Optimus::NetworkManager::getInstance()->m_gameObjectInfo._y );
	pos.z( Optimus::NetworkManager::getInstance()->m_gameObjectInfo._z );

	i_AI.m_gameObject->setPosition( pos );
	i_AI.m_gameObject->setRotation( Optimus::NetworkManager::getInstance()->m_gameObjectInfo.m_rotation );
}

void AI_NetworkController::SetHasFlag( bool i_value )
{
	m_hasFlag = i_value;
	if( m_hasFlag && m_soundPlayed == false )
	{
		m_soundPlayed = true;
		Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/enemyhastheflag.wav" );
	}
}