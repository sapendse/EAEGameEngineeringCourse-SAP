#ifndef MESSAGE_CALLBACK_H
#define MESSAGE_CALLBACK_H

#include "GameObject.h"

namespace Optimus{

	struct GameObject;

	class MessageCallbackI
	{
	public:
		virtual void ProcessMessage( std::string i_message ) = 0;
		virtual ~MessageCallbackI() { }
	};

}

#endif