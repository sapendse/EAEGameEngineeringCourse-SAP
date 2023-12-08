#include "cCamera.h"
#include "UserInput.h"
#include "cTime.h"
#include "..\DebugConsole\DebugConsole.h"

namespace Optimus
{
	cCamera::cCamera() :
               m_position( 0.0f, 0.0f, -50.0f),
               m_lookAt( 0.0f, 0.0f, 0.0f),
               m_up(0.0f, 1.0f, 0.0f)
       {
               
       }

       void cCamera::setPosition( const D3DXVECTOR3 &i_position )
       {
               m_position = i_position;
       }
       void cCamera::setTranslation( const D3DXVECTOR3 &i_offset )
       {
               m_position += i_offset;
       }
       void cCamera::setLookAt( const D3DXVECTOR3 &i_lookAt )
       {
               m_lookAt = i_lookAt;
       }

       const D3DXMATRIX cCamera::GetWorldToViewTransform() const
       {
               D3DXMATRIX transform_worldToView;
               D3DXMatrixTranslation( &transform_worldToView, -m_position.x, -m_position.y, -m_position.z);
               return transform_worldToView;
       }

       bool cCamera::Update()
       {
       /*     D3DXVECTOR3 offset(0.0f, 0.0f, 0.0f);
			if( Optimus::DebugConsole::getInstance()->GetDebugConsoleState() == false )
			{
				if( UserInput::IsKeyPressed( VK_DOWN ))
				{
						offset.z -= 10.0f;
				}
				if( UserInput::IsKeyPressed( VK_RIGHT ))
				{
						offset.x += 10.0f;
				}
				if( UserInput::IsKeyPressed( VK_UP ))
				{
						offset.z += 10.0f;
				}
				if( UserInput::IsKeyPressed( VK_LEFT ))
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
			}

            float unitsPerSecond = 5.0f;

            offset *= (unitsPerSecond * g_time.GetSecondsElapsedThisFrame() );

            setTranslation(offset);*/
			return true;
       }


}