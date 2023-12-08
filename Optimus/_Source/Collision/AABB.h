#ifndef AABB_COLLISION_H
#define AABB_COLLISION_H

#include "Math\Vector3.h"

namespace Optimus{

	struct AABB
	{
		Vector3 m_center;
		float m_rotation, m_halfX, m_halfY, m_halfZ;

		//Constructor
		AABB()
		{
			m_center = Vector3::Zero;
			m_rotation = m_halfX = m_halfY = m_halfZ = 0;
		}

		AABB( const Vector3& i_position, float i_rotation, float i_halfX, float i_halfY, float i_halfZ ) :
			m_center( i_position ),
			m_rotation( i_rotation ),
			m_halfX( i_halfX ),
			m_halfY( i_halfY ),
			m_halfZ( i_halfZ )
		{
		}

		Vector3 getCenter() { return m_center; }
	};
}

#endif