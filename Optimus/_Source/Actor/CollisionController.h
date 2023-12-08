#ifndef COLLISION_CONTROLLER_H
#define COLLISION_CONTROLLER_H

#include "..\Helpers\SharedPointer.h"
#include "GameObject.h"

namespace Optimus{

	struct GameObject;

	class CollisionControllerI
	{
		public:
		virtual ~CollisionControllerI() { }
		virtual void OnCollision( SharedPointer<GameObject>& i_gameObjectSelf, SharedPointer<GameObject> & i_gameObjectTo, Vector3& i_normal ) = 0;
	};

}

#endif
