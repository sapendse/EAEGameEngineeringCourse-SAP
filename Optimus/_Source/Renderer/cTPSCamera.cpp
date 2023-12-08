#include "cTPSCamera.h"
#include "UserInput.h"
#include "cTime.h"
#include "..\Timer\TimeSystem.h"
#include <fstream>
#include <cmath>


#define PI 3.1415

namespace Optimus
{
	void cTPSCamera::GoToFreeForm( bool i_freeForm )
	{
		m_freeFormCamera = true;
	}

	cTPSCamera::cTPSCamera() :
		m_position( 0.0f, 0.0f, -50.0f),
		m_forward( 0.0f, 0.0f, 1.0f),
		m_lookAtPosition( 0.0f, 0.0f, 0.0f ),
		m_up(0.0f, 1.0f, 0.0f),
		m_right( 1.0f, 0.0f, 0.0f ),
		entityCount( 0 ),
		m_collisionHappened_Ground( false ),
		m_collisionHappened_Wall( false ),
		m_playerHeight( 75.0f ),
		m_canSee( false ),
		m_yawRotation( 0.0f ),
		m_freeFormCamera( false )
	{
		
	}

	const D3DXMATRIX cTPSCamera::GetWorldToViewTransform() const
	{
		if( !m_freeFormCamera )
		{
			D3DXMATRIX transform_worldToView;
			D3DXMatrixLookAtLH( &transform_worldToView, &m_position, &m_lookAtPosition, &m_up );
			return transform_worldToView;
		}
		else
		{
			/*D3DXMATRIX transform_worldToView;
            D3DXMatrixTranslation( &transform_worldToView, -m_position.x, -m_position.y, -m_position.z);
            return transform_worldToView;*/
			D3DXMATRIX translationMatrix;
			D3DXMATRIX rotationMatrix;
			D3DXMATRIX worldToViewTransform;
			D3DXMatrixTranslation( &translationMatrix, -m_position.x, -m_position.y, -m_position.z);
			D3DXMatrixRotationY( &rotationMatrix, m_yawRotation );
			worldToViewTransform = translationMatrix * rotationMatrix;
			//D3DXMatrixLookAtLH( &worldToViewTransform, &m_position, &m_lookAtPoint, &m_up);
			return worldToViewTransform;
		}
	}

	void cTPSCamera::MoveTowards()
	{
		if( m_canSee )
			m_position = m_moveTo; //D3DXVECTOR3( m_position * 0.1f + m_moveTo * 0.9f );
		else
			m_position = D3DXVECTOR3( m_position * 0.2f + m_moveTo * 0.8f );
	}

	

	void cTPSCamera::DealWithRotation( float i_rotation )
	{
		float fpsrot = m_yawRotation;
		float phyrot = i_rotation;
		m_yawRotation = i_rotation;
		m_forward = D3DXVECTOR3( m_forward.x * std::cos( phyrot - fpsrot ) - m_forward.z * std::sin( phyrot - fpsrot ), m_forward.y, m_forward.x * std::sin( phyrot - fpsrot ) + m_forward.z * std::cos( phyrot - fpsrot ) );
		m_right = D3DXVECTOR3( m_right.x * std::cos( phyrot - fpsrot ) - m_right.z * std::sin( phyrot - fpsrot ), m_right.y, m_right.x * std::sin( phyrot - fpsrot ) + m_right.z * std::cos( phyrot - fpsrot ) );
	}

	bool cTPSCamera::Update()
	{
		if( m_freeFormCamera )
		{
			m_forward = D3DXVECTOR3( -sin(m_yawRotation), 0.0f, cos(m_yawRotation) );

			D3DXVECTOR3 offset( 0.0f, 0.0f, 0.0f);

			if( UserInput::IsKeyPressed( 'D' ) )
			{
				m_yawRotation -= 0.03f;
			}
			if( UserInput::IsKeyPressed( 'A' ) )
			{
				m_yawRotation += 0.03f;
			}
			if( UserInput::IsKeyPressed( 'W' ) )
			{
				offset += m_forward;
			}
			if( UserInput::IsKeyPressed( 'S' ) )
			{
				offset -= m_forward;
			}
			if( UserInput::IsKeyPressed( VK_SPACE ) )
			{
				m_position.y += 10.0f;
			}
			if( UserInput::IsKeyPressed( VK_CONTROL ) )
			{
				m_position.y -= 10.0f;
			}
			offset *=  (400.0f * g_time.GetSecondsElapsedThisFrame() );

			setTranslation(offset);
		}
		else
		{
			D3DXVECTOR3 offset(0.0f, 0.0f, 0.0f);
			if( UserInput::IsKeyPressed( VK_NUMPAD5 ))
			{
					offset.z -= 10.0f;
			}
			if( UserInput::IsKeyPressed( VK_NUMPAD6 ))
			{
					offset.x += 10.0f;
			}
			if( UserInput::IsKeyPressed( VK_NUMPAD8 ))
			{
					offset.z += 10.0f;
			}
			if( UserInput::IsKeyPressed( VK_NUMPAD4 ))
			{
					offset.x -= 10.0f;
			}
			if( UserInput::IsKeyPressed( VK_SPACE ))
			{
				offset.y += 10.0f;
			}
			if( UserInput::IsKeyPressed( VK_CONTROL ) )
			{
				offset.y -= 10.0f;
			}

            float unitsPerSecond = 5.0f;

            offset *= (unitsPerSecond * g_time.GetSecondsElapsedThisFrame() );

            setTranslation(offset);
		}

		return ( m_collisionHappened_Wall || m_collisionHappened_Ground );
	}

	cTPSCamera::~cTPSCamera()
	{
		//if( m_direct3dDevice )
		//{
		//	delete m_direct3dDevice;
		//	m_direct3dDevice = NULL;
		//}
	}

	void cTPSCamera::Shutdown()
	{
		if( m_instance )
		{
			delete m_instance;
			m_instance = NULL;
		}
	}
}