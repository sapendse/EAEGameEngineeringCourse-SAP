#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "..\Math\Vector3.h"
#include "..\Utilities\New.h"
#include "..\Actor\GameObject.h"
#include "..\Helpers\SharedPointer.h"
#include <vector>

namespace Optimus{
		class Physics{
			std::vector<SharedPointer<GameObject>> m_listOfPhysicsObjects;
			Vector3 tempVel, tempAcc, tempPos;

		public:
			
			static Physics* m_instance;
			static Physics* getInstance(){ 
				if( m_instance == 0 )
					m_instance = OPT_NEW Physics;
				return m_instance;
			}
			Vector3 m_gravity;

			bool Initialize();
			bool Update( F32 deltaTime );
			bool PostUpdate( F32 deltaTime, float i_collisionTime );
			bool Shutdown();
			bool addActor( SharedPointer<GameObject> i_gameObject );
			bool removeActor( SharedPointer<GameObject> i_gameObject );
			void clearList();
		};
}

#endif