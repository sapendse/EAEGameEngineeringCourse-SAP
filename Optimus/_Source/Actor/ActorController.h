#ifndef __ACTOR_CONTROLLER_H
#define __ACTOR_CONTROLLER_H

#include "Actor.h"

namespace Optimus{

class Actor;

class ActorControllerI
{
public:
	virtual ~ActorControllerI() { }
	virtual void UpdateActor( Actor & actor ) = 0;
};

}


#endif