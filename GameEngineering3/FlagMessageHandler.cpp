#include "FlagMessageHandler.h"
#include "Game.h"
#include <sstream>
#include <string>

FlagMessageHandler::FlagMessageHandler( FlagController* i_pFlagController, PlayerController* i_playerController, FlagController* i_eFlagController, AIController* i_AIController )
{
	m_pFlagController	= i_pFlagController;
	m_playerController	= i_playerController;
	m_eFlagController = i_eFlagController;
	m_AIController = i_AIController;
}

void FlagMessageHandler::SetEnemyWinArea( Optimus::Vector3& i_enemyWinAreaPos )
{
	m_enemyWinArea = i_enemyWinAreaPos;
}

void FlagMessageHandler::SetEnemyFlagArea( Optimus::Vector3& i_enemyFlagAreaPos )
{
	m_enemyFlagPos = i_enemyFlagAreaPos;
}

void FlagMessageHandler::ProcessMessage( std::string i_message )
{
	if( "PFlagCaptured" == i_message )
	{
		m_pFlagController->SetFlagCaptured( true );
		m_pFlagController->SetWorldPosition( m_playerController->getPosition() );
		m_playerController->SetHasFlag( true );
	}
	if( "EFlagCaptured" == i_message )
	{
		if( !Optimus::NetworkManager::getInstance()->GetConnected() )
		{
			m_eFlagController->SetFlagCaptured( true );
			Optimus::Vector3 flagWorldPos = m_AIController->GetPosition();
			m_eFlagController->SetWorldPosition( flagWorldPos );
			m_AIController->SetHasFlag( true );
			m_AIController->SetDestination( m_enemyWinArea );
		}
	}
	if( "PlayerWon" == i_message )
	{
		if( m_pFlagController->GetCaptured() )
		{
			if( !Optimus::NetworkManager::getInstance()->GetConnected() )
			{
				Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/youscored.wav" );
				m_pFlagController->ResetFlag( true );
				m_eFlagController->ResetFlag( true );
				m_playerController->IncreaseScore();
				m_playerController->SetHasFlag( false );
				m_AIController->SetHasFlag( false );
				std::string temp = Game::GetInstance()->playerScore;
				std::ostringstream buff;
				buff << m_playerController->GetScore();
				temp = "Player's Score: ";
				temp = temp + buff.str();
				Game::GetInstance()->playerScoreText->SetText( temp );
				m_AIController->SetDestination( m_enemyFlagPos );
			}
			else
			{
				Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/youscored.wav" );
				m_pFlagController->ResetFlag( true );
				m_eFlagController->ResetFlag( true );
				m_playerController->IncreaseScore();
				m_playerController->SetHasFlag( false );
				std::string temp = Game::GetInstance()->playerScore;
				std::ostringstream buff;
				buff << m_playerController->GetScore();
				temp = "Player's Score: ";
				temp = temp + buff.str();
				Game::GetInstance()->playerScoreText->SetText( temp );
			}
		}
	}
	if( "EnemyWon" == i_message )
	{
		if( !Optimus::NetworkManager::getInstance()->GetConnected() )
		{
			if( m_AIController->GetHasFlag() )
			{
				Optimus::Vector3 dist = m_AIController->GetPosition() - m_eFlagController->m_flagWorldPosition;
				float d = sqrt( Optimus::Vector3::magSqr( dist ) );
				if( d < 25.0f )
				{
					m_eFlagController->ResetFlag( true );
					m_pFlagController->ResetFlag( true );
					Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/enemyscored.wav" );
					m_AIController->IncreaseScore();
					m_AIController->SetHasFlag( false );
					m_playerController->SetHasFlag( false );
					std::string temp = Game::GetInstance()->playerScore;
					std::ostringstream buff;
					buff << m_AIController->GetScore();
					temp = "AI's Score: ";
					temp = temp + buff.str();
					Game::GetInstance()->enemyScoreText->SetText( temp );
					m_AIController->SetDestination( m_enemyFlagPos );
				}
				else
				{
					m_AIController->SetHasFlag( false );
					m_AIController->SetDestination( m_enemyFlagPos );
				}
			}
		}
		else
		{
			if( Optimus::NetworkManager::getInstance()->m_playerInfo.m_hasFlag )
			{
				m_eFlagController->ResetFlag( true );
				m_pFlagController->ResetFlag( true );
				Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/enemyscored.wav" );
				Optimus::NetworkManager::getInstance()->m_playerInfo.m_score++;
				Optimus::NetworkManager::getInstance()->m_playerInfo.m_hasFlag = false;
				m_playerController->SetHasFlag( false );
				std::string temp = Game::GetInstance()->playerScore;
				std::ostringstream buff;
				buff << Optimus::NetworkManager::getInstance()->m_playerInfo.m_score;
				temp = "AI's Score: ";
				temp = temp + buff.str();
				Game::GetInstance()->enemyScoreText->SetText( temp );
			}
		}
	}
	if( "EnemyTaggedPlayer" == i_message )
	{
		if( m_playerController->GetHasFlag() )
		{
			m_pFlagController->ResetFlag( true );
			m_playerController->SetHasFlag( false );
			Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/nuclear-alarm.wav" );
		}
		if( !Optimus::NetworkManager::getInstance()->GetConnected() )
		{
			if( m_AIController->GetHasFlag() )
			{
				m_eFlagController->ResetFlag( true );
				m_AIController->SetHasFlag( false );
				Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/nuclear-alarm.wav" );
				m_AIController->SetDestination( m_enemyFlagPos );
			}
		}
		else
		{
			m_eFlagController->ResetFlag( true );
			Optimus::NetworkManager::getInstance()->m_playerInfo.m_hasFlag = false;
			Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/nuclear-alarm.wav" );
		}
	}
	if( "PlayerTaggedEnemy" == i_message )
	{
		if( !Optimus::NetworkManager::getInstance()->GetConnected() )
		{
			if( m_AIController->GetHasFlag() )
			{
				m_eFlagController->ResetFlag( true );
				m_AIController->SetHasFlag( false );
				Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/nuclear-alarm.wav" );
				m_AIController->SetDestination( m_enemyFlagPos );
			}
		}
		else
		{
			m_eFlagController->ResetFlag( true );
			Optimus::NetworkManager::getInstance()->m_playerInfo.m_hasFlag = false;
			Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/nuclear-alarm.wav" );
		}
		if( m_playerController->GetHasFlag() )
		{
			m_pFlagController->ResetFlag( true );
			m_playerController->SetHasFlag( false );
			Optimus::SoundManager::getInstance()->PlayOnce( "../Assets/Sounds/nuclear-alarm.wav" );
		}
	}
}