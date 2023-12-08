#ifndef _NETWORK_MANAGER_H
#define _NETWORK_MANAGER_H

#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#include "BitStream.h"
#include "MessageIdentifiers.h"
#include "..\Utilities\New.h"
#include "..\Math\Vector3.h"

#define SERVERPORT 60000

namespace Optimus
{
	struct GameObject_Minimal
	{
		float _x, _y, _z;
		float m_rotation;
	};

	struct PlayerInfo
	{
		float m_stamina;
		int m_score;
		bool m_hasFlag;
	};

	enum MessageType
	{
		GAMEOBJECT_MINIMAL = ID_USER_PACKET_ENUM + 1, //Capitalised to avoid conflict
		PLAYERINFO
	};

	class NetworkManager
	{
	public:
		RakNet::RakPeerInterface*	m_peer;						//Peer Instance provided by RakNet
		RakNet::SystemAddress		m_clientAddress;			//Address of Client
		GameObject_Minimal			m_gameObjectInfo;
		PlayerInfo					m_playerInfo;
		int							m_maxConnectionsAllowed;	//Should be less than 10
		bool						m_isServer;
		bool						m_isConnected;

		NetworkManager();

		static NetworkManager* m_instance;
		static NetworkManager* getInstance(){ 
			if( m_instance == 0 )
				m_instance = OPT_NEW NetworkManager();
			return m_instance;
		}

		//Loop Functions
		void Initialize();
		void Update();
		void Shutdown();
		
		//Network Functionality
		void SetConnectionsAllowed( int i_connectionsAllowed );
		void CreateServer();
		void ConnectToServer( char* i_IPAddress );
		void SendGameObject( GameObject_Minimal i_gObject );
		void SendPlayerInfo( PlayerInfo i_pInfo );
		void ReceiveMessage();
		
		bool GetConnected() { return m_isConnected; }
	};
}

#endif