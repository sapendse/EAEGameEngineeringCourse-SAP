#ifndef CTPS_CAMERA_H
#define CTPS_CAMERA_H


#include <d3dx9math.h>
#include "..\Actor\Actor.h"
#include "VertexFormat.h"
#include "..\Utilities\DebugLines.h"

namespace Optimus
{

	class cTPSCamera
	{
	public:		
		D3DXVECTOR3								m_position;
		D3DXVECTOR3								m_up;
		D3DXVECTOR3								m_forward;
		D3DXVECTOR3								m_right;
		D3DXVECTOR3								m_lookAtPosition;
		D3DXVECTOR3								m_moveTo;
		float									m_yawRotation, m_playerHeight;
		unsigned int							entityCount;
		bool									m_canSee, m_freeFormCamera, m_collisionHappened_Ground, m_collisionHappened_Wall;	

		//Constructor
        cTPSCamera();

		//Destructor
		~cTPSCamera();

	private:
		static cTPSCamera*						m_instance;
	public:
		static cTPSCamera*	getInstance()
		{
			if( m_instance == 0 )
				m_instance = OPT_NEW cTPSCamera();
			return m_instance;
		}

		//Update method
        bool				Update();
		void				GoToFreeForm( bool i_freeForm );
		void				MoveTowards();
		void				DealWithRotation( float i_rotation );

		//Shutdown Method
		void Shutdown();
		
		//Setters
		void				setPosition( float i_x, float i_y, float i_z ) { m_position.x = i_x; m_position.y = i_y; m_position.z = i_z; }
		void				setFoward( float i_x, float i_y, float i_z ) { m_forward.x = i_x; m_forward.y = i_y; m_forward.z = i_z; }
		void				setRight( float i_x, float i_y, float i_z ) { m_right.x = i_x; m_right.y = i_y; m_right.z = i_z; }
		void				setRotation( float i_rotation ) { m_yawRotation = i_rotation; }
        void				setTranslation( const D3DXVECTOR3 &i_offset ) { m_position += i_offset; }
        void				setForward( const D3DXVECTOR3 &i_forward ) { m_forward = i_forward; }
		void				setMoveTo( const D3DXVECTOR3& i_moveTo ) { m_moveTo = i_moveTo; }
		void				setLookAtPosition( const D3DXVECTOR3& i_lookAtPosition ) { m_lookAtPosition = i_lookAtPosition; }

		//Getters
		D3DXVECTOR3			getPosition() { return m_position; }
		D3DXVECTOR3			getForward()  { return m_forward; }
		D3DXVECTOR3			getRight()	  { return m_right; }
		float				getRotation() { return m_yawRotation; }
        const D3DXMATRIX	GetWorldToViewTransform() const;
		bool				getGroundCollision() { return m_collisionHappened_Ground; }

		//Draw Methods
		void				DrawForwardRaycast( Optimus::Vector3& i_start, Optimus::Vector3& i_end );
		void				DrawOctreeBox( Optimus::Vector3& i_minBound, Optimus::Vector3& i_maxBound );
	};
	
}


#endif