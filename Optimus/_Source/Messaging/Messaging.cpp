#include "Messaging.h"
#include <assert.h>


void Optimus::Messaging::AddMesage( std::string i_message )
{
	unsigned int index = 0;
	for( index = 0; index< m_messages.size(); index++ )
	{
		if( m_messages[index].m_message == i_message )
		{
			break;
		}
	}	
	assert( index == m_messages.size() );

	Message temp;
	temp.m_message = i_message;

	m_messages.push_back( temp );
}

void Optimus::Messaging::Subscribe( std::string i_message, MessageCallbackI* i_messageHandler )
{
	unsigned int index = 0;
	for( index = 0; index< m_messages.size(); index++ )
	{
		if( m_messages[index].m_message == i_message )
		{
			m_messages[index].m_listOfMessageHandlers.push_back( i_messageHandler );
			break;
		}
	}
	assert( index < m_messages.size() );
}

void Optimus::Messaging::Unsubscribe( std::string i_message, MessageCallbackI* i_messageHandler )
{
	bool stepOut = false;

	for( unsigned int i = 0; i< m_messages.size(); i++ )
	{
		if( m_messages[i].m_message == i_message )
		{
			for( unsigned int j = 0; j < m_messages[i].m_listOfMessageHandlers.size(); j++ )
			{
				if( m_messages[i].m_listOfMessageHandlers[j] == i_messageHandler )
				{
					m_messages[i].m_listOfMessageHandlers.erase( m_messages[i].m_listOfMessageHandlers.begin() + j );
					stepOut = true;
					break;
				}
			}
		}
		if( stepOut == true )
			break;
	}
}

void Optimus::Messaging::UseMessage( std::string i_message )
{
	for( unsigned int i =0; i< m_messages.size(); i++ )
	{
		if( m_messages[i].m_message == i_message )
		{
			for( unsigned int j =0; j< m_messages[i].m_listOfMessageHandlers.size(); j++ )
			{
				m_messages[i].m_listOfMessageHandlers[j]->ProcessMessage( i_message );
			}
			break;
		}
	}
}