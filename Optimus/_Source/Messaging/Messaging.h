#ifndef MESSAGING_H
#define MESSAGING_H

#include "..\Actor\MessageCallback.h"
#include "..\Utilities\New.h"
#include <vector>
#include <string>

namespace Optimus{

	struct Message{
		std::string m_message;
		std::vector<MessageCallbackI*> m_listOfMessageHandlers;
	};

	class Messaging
	{

	public:
		static Messaging* m_instance;
		static Messaging* getInstance(){ 
			if( m_instance == 0 )
				m_instance = OPT_NEW Messaging;
			return m_instance;
		}
		void AddMesage( std::string i_message );
		void Subscribe( std::string i_message, MessageCallbackI* i_messageHandler );
		void Unsubscribe( std::string i_message, MessageCallbackI* i_messageHandler );
		void UseMessage( std::string i_message );
		std::vector<Message> m_messages;
	};
}

#endif