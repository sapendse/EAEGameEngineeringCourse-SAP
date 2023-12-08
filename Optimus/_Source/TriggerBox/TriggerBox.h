#ifndef _TRIGGER_BOX_H
#define _TRIGGER_BOX_H

#include <d3dx9math.h>
#include "..\Actor\GameObject.h"
#include "..\Helpers\SharedPointer.h"
#include "..\Math\Vector3.h"
#include "..\Utilities\DebugLines.h"

namespace Optimus
{
	class TriggerBox : public TriggerCallbackI
	{
		Vector3				m_dimensions, m_center;
		IDirect3DDevice9*	m_direct3dDevice;
		Line*				line_vertices;
		bool				m_drawable;

	public:
		TriggerBox();
		TriggerBox( TriggerBox& i_rhs );
		TriggerBox( Optimus::Vector3& i_center, Optimus::Vector3& i_dimensions );
		~TriggerBox();

		void				SetDrawable( bool i_drawable );
		virtual void		OnEnter( Optimus::SharedPointer<Optimus::GameObject> i_enter, Optimus::SharedPointer<Optimus::GameObject> i_trigger );
		virtual void		OnExit( Optimus::SharedPointer<Optimus::GameObject> i_enter, Optimus::SharedPointer<Optimus::GameObject> i_trigger );
		void				Initialize();
		void				Draw( IDirect3DDevice9* i_direct3dDevice );
		void				Shutdown();
		Vector3				GetCenter();
		Vector3				GetDimensions();
		void				SetPosition( const Optimus::Vector3& i_position );
	};

}
#endif