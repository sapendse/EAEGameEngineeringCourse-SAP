#pragma once

#ifndef _FPSCAMERA_H
#define _FPSCAMERA_H

#include <d3dx9math.h>
#include "..\Actor\Actor.h"
#include "VertexFormat.h"
#include "..\Utilities\New.h"
#include "..\World\OctreeBuilder.h"
#include "..\Utilities\DebugLines.h"

namespace Optimus
{
	struct s_mesh
	{
		unsigned int vertexCount, indexCount;
		s_vertex* myVertex;
		unsigned int* myIndex;
	};

	class cFPSCamera
	{
		Optimus::SharedPointer<Optimus::Actor> m_player;
		D3DXVECTOR3			m_position;
		D3DXVECTOR3			m_up;
		D3DXVECTOR3			m_lookAt;
		D3DXVECTOR3			m_right;
		float				m_yawRotation, m_playerHeight;
		s_mesh*				myMeshes;
		unsigned int		entityCount;
		IDirect3DDevice9* m_direct3dDevice;
		bool				m_collisionHappened_Ground, m_collisionHappened_Wall;

		public:
		//Constructor
        cFPSCamera();

		static cFPSCamera* m_instance;
		static cFPSCamera* getInstance()
		{
			if( m_instance == 0 )
				m_instance = OPT_NEW cFPSCamera();
			return m_instance;
		}

		//Initialize Method
		bool Initialize( IDirect3DDevice9* i_direct3dDevice );
		void ReadMeshData();
		void ReadData();
		void FindTrianglesToCollide( Optimus::Vector3& a, Optimus::Vector3& b, OctreeBuilder::OctreeNode* i_node, std::vector<OctreeBuilder::Triangle>& i_triangleList  );

		//Update method
        bool Update();
		void CollisionDetection();
		void CollisionResponse( int i_type, Optimus::Vector3& i_ray, Optimus::Vector3& i_first, Optimus::Vector3& i_second, Optimus::Vector3& i_third, float u, float v, float w, float t );
		void UpdateInfo( Optimus::Vector3& i_pos, Optimus::Vector3& i_lookAt, Optimus::Vector3& i_speed, Optimus::Vector3& i_groundRay, Optimus::Vector3& i_for, Optimus::Vector3& i_back );

		//Setters
		void				setPosition( float i_x, float i_y, float i_z ) { m_position.x = i_x; m_position.y = i_y; m_position.z = i_z; }
		void				setLookAt( float i_x, float i_y, float i_z ) { m_lookAt.x = i_x; m_lookAt.y = i_y; m_lookAt.z = i_z; }
		void				setRight( float i_x, float i_y, float i_z ) { m_right.x = i_x; m_right.y = i_y; m_right.z = i_z; }
		void				setRotation( float i_rotation ) { m_yawRotation = i_rotation; }
        void				setTranslation( const D3DXVECTOR3 &i_offset ) { m_position += i_offset; }
        void				setLookAt( const D3DXVECTOR3 &i_lookAt ) { m_lookAt = i_lookAt; }
		void				setPlayer( Optimus::SharedPointer<Optimus::Actor>& i_player ) { m_player = i_player; }

		//Getters
		D3DXVECTOR3			getPosition() { return m_position; }
		D3DXVECTOR3			getLookAt()	  { return m_lookAt; }
		D3DXVECTOR3			getRight()	  { return m_right; }
		float				getRotation() { return m_yawRotation; }
        const D3DXMATRIX	GetWorldToViewTransform() const;
		bool				getGroundCollision() { return m_collisionHappened_Ground; }
	};
	
}

#endif