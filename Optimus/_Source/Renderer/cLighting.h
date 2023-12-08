#pragma once

#ifndef _LIGHTS_H
#define _LIGHTS_H

#include <d3dx9math.h>

namespace Optimus{

	class cLighting{

		D3DXVECTOR3		m_position , m_up;
		D3DXVECTOR4		m_params;
		float			m_attFac;

	public:
		//Default Constructor
		cLighting();

		//Destructor
		~cLighting();

		//Getters
		D3DXVECTOR3		getPosition()	{ return m_position; }
		D3DXVECTOR3		getUp()			{ return m_up; }
		D3DXVECTOR4		getParams()		{ return m_params; }
		float			getAttFac()		{ return m_attFac; }

		//Setters
		void			setPosition( D3DXVECTOR3& i_position );
		void			setTranslation( D3DXVECTOR3& i_offset );
		void			setParams( D3DXVECTOR4& i_params );
		void			setAttFac( float i_attFac );

		//Update
		void Update();
	};

}

#endif