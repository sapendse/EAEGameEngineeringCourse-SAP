#ifndef _GAME_H
#define _GAME_H

#include "..\Optimus\_Source\Loop\Optimus.h"
#include "..\Optimus\_Source\Networking\NetworkManager.h"

class Game
{
public:
	std::string				playerScore;
	std::string				enemyScore;
	std::string				playerStamina;
	std::string				enemyStamina;
	Optimus::cText*			playerScoreText;
	Optimus::cText*			enemyScoreText;
	Optimus::cText*			playerStaminaText;
	Optimus::cText*			enemyStaminaText;
	bool					m_modeSwitched;
	Game() { m_modeSwitched = false; }

	static Game* m_instance;
	static Game* GetInstance(){ 
		if( m_instance == 0 )
			m_instance = OPT_NEW Game();
		return m_instance;
	}

	//void ReadConfigFile();
	std::string getGameName();
	unsigned int getWidth();
	unsigned int getHeight();
	bool parseDynamicFile( const char* i_fileName, std::vector<Optimus::cMesh*>& i_meshVector, std::vector<Optimus::Vector3>& _pos, std::vector<Optimus::Vector3>& _size );
	bool Initialize();
	bool Update();
	bool ShutDown();
	void SyncGames();
};

#endif