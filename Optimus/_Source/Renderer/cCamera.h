#pragma once

#ifndef _CAMERA_H
#define _CAMERA_H

#include <d3dx9math.h>

namespace Optimus
{

	class cCamera
	{
		D3DXVECTOR3 m_position;
		D3DXVECTOR3 m_lookAt;
		D3DXVECTOR3 m_up;

		public:
		//Constructor
        cCamera();

		//Update method
        bool Update();

		//Setters
        void setPosition( const D3DXVECTOR3 &i_position );
        void setTranslation( const D3DXVECTOR3 &i_offset );
        void setLookAt( const D3DXVECTOR3 &i_lookAt );

		//Getters
		D3DXVECTOR3 getPosition() { return m_position; }
        const D3DXMATRIX GetWorldToViewTransform() const;
	};
	
}

#endif