#ifndef COLLISION_H
#define COLLISION_H

#include "..\Math\Vector3.h"
#include "..\Math\Matrix.h"
#include "..\Utilities\New.h"
#include "..\Actor\GameObject.h"
#include "..\Timer\TimeSystem.h"
#include "..\Physics\Physics.h"
#include "..\Helpers\SharedPointer.h"
#include "AABB.h"
#include <vector>
#include "..\Profiling\Profiling.h"
#include "..\World\OctreeBuilder.h"

namespace Optimus{

	struct AABB;

	class Collision{
		std::vector<SharedPointer<GameObject>> m_listOfCollidableObjects;
		bool collision;
		float goodTimeToStop;
		Vector3 normal;

	public: 

		unsigned int m_instanceOfCollision;
		AABB aabb1, aabb2;
		static Collision* m_instance;
		static Collision* getInstance(){ 
			if( m_instance == 0 )
				m_instance = OPT_NEW Collision;
			return m_instance;
		}

		Vector3 findRelativeVelocity( Vector3& i_first, Vector3& i_second );
		void setAABB( unsigned int i, unsigned int j, F32 deltaTime );
		Vector3 setOOBB( unsigned int i_count, unsigned int i, unsigned int j, F32 deltaTime );
		void resetAABB( unsigned int i, unsigned int j );
		bool checkAABBCollision( Vector3& i_prevPos );
		bool getCollisionState() { return collision; }
		float getCollideTime() { return goodTimeToStop; }
		bool Initialize();
		bool Update( F32 deltaTime );
		bool Shutdown();
		bool addActor( SharedPointer<GameObject> i_gameObject );
		bool removeActor( SharedPointer<GameObject> i_gameObject );
		void clearList();
		void setNormal( float i_x, float i_y, float i_z );
		void respondToCollision( unsigned int i, unsigned int j );
		int IntersectSegmentTriangle( Vector3& p, Vector3& q, Vector3& a, Vector3& b, Vector3& c, float &u, float &v, float &w, float &t );
		int IntersectSegmentAABB(Vector3& p1, Vector3& p2, Vector3& center, Vector3& halfDim);
		void CameraCollisionDetection( unsigned int i_index );
		void PlayerCollisionDetection( unsigned int i_index );
		void FindTrianglesToCollide( Optimus::Vector3& a, Optimus::Vector3& b, OctreeBuilder::OctreeNode* i_node, std::vector<OctreeBuilder::Triangle>& i_triangleList  );
		void UpdateInfo( unsigned int i_index, Optimus::Vector3& i_pos, Optimus::Vector3& i_lookAt, Optimus::Vector3& i_speed, Optimus::Vector3& i_groundRay, Optimus::Vector3& i_for, Optimus::Vector3& i_back );
	};
}

#endif