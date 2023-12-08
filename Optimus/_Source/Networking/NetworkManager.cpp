#include "NetworkManager.h"

void Optimus::NetworkManager::Initialize()
{
	m_isServer  = false;
	m_peer		= RakNet::RakPeerInterface::GetInstance();
	m_maxConnectionsAllowed = 10; //Just in case.
	m_isConnected = false;
}

Optimus::NetworkManager::NetworkManager()
{
	m_gameObjectInfo.m_rotation = 0.0f;
	m_gameObjectInfo._x = 0.0f;
	m_gameObjectInfo._y = 0.0f;
	m_gameObjectInfo._z = 0.0f;

	m_playerInfo.m_hasFlag = false;
	m_playerInfo.m_score = 0;
	m_playerInfo.m_stamina = 100.0f;
}

void Optimus::NetworkManager::Update()
{
	if( !m_isConnected )
	{
		RakNet::ConnectionState result = m_peer->GetConnectionState( m_clientAddress );
		if(result == RakNet::ConnectionState::IS_CONNECTED)
			m_isConnected = true;
		else if(result == RakNet::ConnectionState::IS_NOT_CONNECTED)
			m_isConnected = false;
	}
	ReceiveMessage();
}

void Optimus::NetworkManager::Shutdown()
{
	if( m_peer != NULL )
	{
		RakNet::RakPeerInterface::DestroyInstance(m_peer);
	}
	if( m_instance != NULL )
	{
		delete m_instance;
		m_instance = NULL;
	}
}

void Optimus::NetworkManager::SetConnectionsAllowed( int i_connectionsAllowed )
{
	if( m_maxConnectionsAllowed < 10 )
		m_maxConnectionsAllowed = i_connectionsAllowed;
	else
		return;
}

void Optimus::NetworkManager::CreateServer()
{
	RakNet::SocketDescriptor sd(SERVERPORT,0);
	m_peer->Startup(m_maxConnectionsAllowed, &sd, 1);
	m_isServer = true;
	m_peer->SetMaximumIncomingConnections( 10 );
}

void Optimus::NetworkManager::ConnectToServer( char* i_IPAddress )
{
	m_peer->Startup(1, &RakNet::SocketDescriptor(), 1);
	RakNet::ConnectionAttemptResult result= m_peer->Connect(i_IPAddress, SERVERPORT, 0, 0, 0); //Let's hope connection is good.
}
void Optimus::NetworkManager::SendGameObject( GameObject_Minimal i_gObject )
{
	RakNet::BitStream streamToSend;
	streamToSend.Write( (RakNet::MessageID)(GAMEOBJECT_MINIMAL) );
	streamToSend.Write( i_gObject );

	if( m_isServer )
	{
		m_peer->Send( &streamToSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_clientAddress, false );
	}
	else
	{
		RakNet::AddressOrGUID addOrGUID;
		addOrGUID.rakNetGuid = m_peer->GetGUIDFromIndex(0);
		addOrGUID.systemAddress = m_peer->GetSystemAddressFromIndex( 0 );
		m_peer->Send( &streamToSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addOrGUID, false );
	}
}

void Optimus::NetworkManager::SendPlayerInfo( PlayerInfo i_gObject )
{
	RakNet::BitStream streamToSend;
	streamToSend.Write( (RakNet::MessageID)(PLAYERINFO) );
	streamToSend.Write( i_gObject );

	if( m_isServer )
	{
		m_peer->Send( &streamToSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_clientAddress, false );
	}
	else
	{
		RakNet::AddressOrGUID addOrGUID;
		addOrGUID.rakNetGuid = m_peer->GetGUIDFromIndex(0);
		addOrGUID.systemAddress = m_peer->GetSystemAddressFromIndex( 0 );
		m_peer->Send( &streamToSend, HIGH_PRIORITY, RELIABLE_ORDERED, 0, addOrGUID, false );
	}
}

void Optimus::NetworkManager::ReceiveMessage()
{
	RakNet::Packet* tempPacket = NULL;

	for( tempPacket= m_peer->Receive(); tempPacket; m_peer->DeallocatePacket(tempPacket), tempPacket= m_peer->Receive() )
	{
		switch( tempPacket->data[0] )
		{
			case GAMEOBJECT_MINIMAL:
			{
				RakNet::BitStream streamReceieved( tempPacket->data, tempPacket->length, false );
				streamReceieved.IgnoreBytes( sizeof( RakNet::MessageID ) );
				streamReceieved.Read( m_gameObjectInfo );
				break;
			}
			case PLAYERINFO :
			{
				RakNet::BitStream streamReceieved( tempPacket->data, tempPacket->length, false );
				streamReceieved.IgnoreBytes( sizeof( RakNet::MessageID ) );
				streamReceieved.Read( m_playerInfo );
				break;
			}
			default:
			{
				printf( "FUCKTHISSHIT", tempPacket->data[0] );
				m_clientAddress = tempPacket->systemAddress;
				break;
			}
		}
	}
}