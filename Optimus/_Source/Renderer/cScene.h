#ifndef _CSCENE_H
#define _CSCENE_H

#include <vector>
#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9.h>
#include <d3dx9math.h>

#include "cMesh.h"
#include "..\Math\Vector3.h"
#include "..\Utilities\New.h"
#include "cTPSCamera.h"
#include "cFPSCamera.h"
#include "cText.h"
#include "cSprite.h"
#include "../TriggerBox/TriggerBox.h"
#include "../Utilities/DebugLines.h"

namespace Optimus
{
	class cSceneHandler
	{
		//Constructor
		cSceneHandler();
		
	public:
		float								m_fovAngle;
		std::vector<unsigned int>			m_entityCount;
		std::vector<cMesh*>					m_entities;
		std::vector<cMesh*>					m_opaqueEnts;
		std::vector<cMesh*>					m_transEnts;
		std::vector<Optimus::cSprite*>		m_sprites;
		std::vector<Optimus::cText*>		m_texts;
		std::vector<Optimus::TriggerBox*>	m_triggerBoxes;
		std::vector<Optimus::DebugLines*>	m_debugLines;
		cLighting*							m_lighting;

		IDirect3DDevice9*					m_direct3dDevice;
		bool								m_isWireframe;

		static cSceneHandler*				m_instance;
		static cSceneHandler* getInstance()
		{
			if( m_instance == 0 )
				m_instance = OPT_NEW cSceneHandler();
			return m_instance;
		}

		void GoToWireframe( bool i_boolValue );

		//Functions
		void divideOpaqueTrans();
		void sortOpaqueEntities();
		void sortTransEntities();
		bool Initialise( IDirect3DDevice9* i_direct3dDevice );
		bool parseSceneFile( const char* i_name );
		bool parseDynamicFile( const char* i_name );
		void Update();
		void Draw();
		void ShutDown();
		int  CompareMaterials( cMesh* i_ent1, cMesh* i_ent2 );
		void AddCube( Optimus::Vector3 i_center, Optimus::Vector3 i_dimensions, int i_solid );
		void AddSphere( Optimus::Vector3 i_center, float i_radius, int i_solid );
		std::vector<cMesh*> AddMesh( Optimus::Vector3& i_center );
		Optimus::cSprite* AddSprite( const char* i_textureFilePath, D3DCOLOR i_color, float i_posX, float i_posY );
		void AddLine( float i_startX, float i_startY, float i_length );
		Optimus::cText* AddText( unsigned int i_height, unsigned int i_width, const char* i_text, int i_left, int i_top, int i_right, int i_bottom, D3DCOLOR i_color, bool i_drawText );
		Optimus::TriggerBox* AddTriggerBox( Optimus::Vector3& i_center, Optimus::Vector3& i_dimensions );
		void AddDebugLine( const Optimus::Vector3& i_start, const Optimus::Vector3& i_end, DWORD i_colorLine );
		void AddDebugOctreeBox( const Optimus::Vector3& i_minBound, const Optimus::Vector3& i_maxBound );
		void DeleteDebugLines();
	};
}

#endif