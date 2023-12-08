#include "cScene.h"
#include "..\Utilities\New.h"
#include <fstream>
#include <string>
#include "../DebugConsole/DebugConsole.h"
#include <map>
#include "..\PathFinding\WayPointGraph.h"

Optimus::cSceneHandler* Optimus::cSceneHandler::m_instance = NULL;

void WireFrameIt( const char* i_menuName, bool i_boolValue )
{
	Optimus::cSceneHandler::getInstance()->GoToWireframe( i_boolValue );
}

void RendererMenu()
{

}

void CameraMovement()
{

}

void FreeFormCamera( const char* i_menuName, bool i_boolValue )
{
	Optimus::cTPSCamera::getInstance()->GoToFreeForm( i_boolValue );
}

namespace Optimus
{
	cSceneHandler::cSceneHandler() :
	m_direct3dDevice( NULL ),
	m_fovAngle( D3DX_PI / 4 )
	{
		m_lighting = OPT_NEW cLighting();
	}

	bool cSceneHandler::Initialise( IDirect3DDevice9* i_direct3dDevice )
	{
		m_direct3dDevice = i_direct3dDevice;
		return true;
	}

	void cSceneHandler::Update()
	{
		if( cTPSCamera::getInstance()->Update() )
		{
			if( m_transEnts.size() != 0 )
				sortTransEntities();
		}
		m_lighting->Update();
	}

	void cSceneHandler::ShutDown()
	{
		if( m_lighting )
		{
			delete m_lighting;
			m_lighting = NULL;
		}

		cTPSCamera::getInstance()->Shutdown();

		if( m_direct3dDevice )
		{
			m_direct3dDevice->Release();
			m_direct3dDevice = NULL;
		}

		if( m_lighting )
		{
			delete m_lighting;
			m_lighting = NULL;
		}

		unsigned int temp = m_entities.size();
		for( unsigned int i = 0; i < temp; i++ )
		{
			//m_entities[i]->~cMesh();
			if( m_entities[i] )
			{
				delete m_entities[i];
				m_entities[i] = NULL;
			}
		}

		temp = m_triggerBoxes.size();
		for( unsigned int i = 0; i < temp; i++ )
		{
			if( m_triggerBoxes[i] )
			{
				delete m_triggerBoxes[i];
				m_triggerBoxes[i] = NULL;
			}
		}

		//temp = m_texts.size();
		//for( unsigned int i = 0; i < temp; i++ )
		//{
		//	if( m_texts[i] )
		//	{
		//		delete m_texts[i];
		//		m_texts[i] = NULL;
		//	}
		//}
		DeleteDebugLines();
		//for( unsigned int i = 0; i < m_opaqueEnts.size(); i++ )
		//{
		//	m_opaqueEnts[i]->~cMesh();
		//}

		//for( unsigned int i = 0; i < m_texts.size(); i++ )
		//{
		//	m_texts[i]->~cText();
		//}

		//for( unsigned int i = 0; i < m_sprites.size(); i++ )
		//{
		//	m_sprites[i]->~cSprite();
		//}

		//m_opaqueEnts.~vector();

		//m_transEnts.~vector();
		WayPointGraph::GetInstance()->ShutDown();
		if( m_instance )
		{
			delete m_instance;
			m_instance = NULL;
		}
	}

	void cSceneHandler::DeleteDebugLines()
	{
		for( unsigned int i = 0; i < m_debugLines.size(); i++ )
		{
			if( m_debugLines[i] )
			{
				delete m_debugLines[i];
				m_debugLines[i] = NULL;
			}
		}
	}

	void cSceneHandler::AddCube( Optimus::Vector3 i_center, Optimus::Vector3 i_dimensions, int i_solid )
	{
		cMesh* newCube = OPT_NEW cMesh();
		newCube->Initialize( m_direct3dDevice );
		newCube->parseFiles( "../Assets/Meshes/cube.txt", 0 );
		newCube->setPosition( D3DXVECTOR3( i_center.x(), i_center.y(), i_center.z() ) );
		newCube->setSolid( i_solid );
		m_entities.push_back( newCube );
	}

	std::vector<cMesh*> cSceneHandler::AddMesh( Optimus::Vector3& i_center )
	{
		cMesh* newCube = OPT_NEW cMesh();
		std::vector<cMesh*> temp;
		newCube->Initialize( m_direct3dDevice );
		newCube->parseFiles( "../Assets/Meshes/player.bin", 0 );
		//newCube->setPosition( D3DXVECTOR3( i_center.x(), i_center.y(), i_center.z() ) );
		newCube->setSolid( true );
		newCube->m_player = true;
		m_entities.push_back( newCube );
		temp.push_back( newCube );
		m_opaqueEnts.clear();
		m_transEnts.clear();
		divideOpaqueTrans();
		sortOpaqueEntities();
		sortTransEntities();
		m_entities.~vector();
		return temp;
	}

	void cSceneHandler::AddSphere( Optimus::Vector3 i_center, float i_radius, int i_solid )
	{
		cMesh* newCube = OPT_NEW cMesh();
		newCube->Initialize( m_direct3dDevice );
		newCube->parseFiles( "Assets/Meshes/sphere.txt", 0 );
		newCube->setPosition( D3DXVECTOR3( i_center.x(), i_center.y(), i_center.z() ) );
		newCube->setSolid( i_solid );
		m_entities.push_back( newCube );
	}

	Optimus::cSprite* cSceneHandler::AddSprite( const char* i_textureFilePath, D3DCOLOR i_color, float i_posX, float i_posY )
	{
		D3DXVECTOR3 tempPos;
		tempPos.x = i_posX;
		tempPos.y = i_posY;
		tempPos.z = 0.0f;

		Optimus::cSprite* tempSprite = OPT_NEW Optimus::cSprite( m_direct3dDevice, i_textureFilePath, i_color, tempPos );

		m_sprites.push_back( tempSprite );

		return tempSprite;
	}

	Optimus::cText* cSceneHandler::AddText( unsigned int i_height, unsigned int i_width, const char* i_text, int i_left, int i_top, int i_right, int i_bottom, D3DCOLOR i_color, bool i_drawText )
	{
		Optimus::cText* tempText = OPT_NEW Optimus::cText( m_direct3dDevice, i_text, i_height, i_width, i_left, i_top, i_right, i_bottom, i_color, i_drawText );

		m_texts.push_back( tempText );

		return tempText;
	}

	Optimus::TriggerBox* cSceneHandler::AddTriggerBox( Optimus::Vector3& i_center, Optimus::Vector3& i_dimensions )
	{
		TriggerBox* tempTriggerBox = OPT_NEW TriggerBox( i_center, i_dimensions );
		tempTriggerBox->Initialize();

		m_triggerBoxes.push_back( tempTriggerBox );

		return tempTriggerBox;
	}

	void cSceneHandler::AddDebugLine( const Optimus::Vector3& i_start, const Optimus::Vector3& i_end, DWORD i_colorLine )
	{
		DebugLines* tempDebugLine = OPT_NEW DebugLines( Optimus::v_Zero, Optimus::v_Zero, i_start, i_end, i_colorLine );
		tempDebugLine->Initialize();
		
		m_debugLines.push_back( tempDebugLine );
	}

	void cSceneHandler::AddDebugOctreeBox( const Optimus::Vector3& i_minBound, const Optimus::Vector3& i_maxBound )
	{
		DebugLines* tempDebugBox = OPT_NEW DebugLines( i_minBound, i_maxBound, Optimus::v_Zero, Optimus::v_Zero, D3DCOLOR_XRGB( 0, 255, 255 ) );
		tempDebugBox->Initialize();

		m_debugLines.push_back( tempDebugBox );
	}

	void cSceneHandler::sortOpaqueEntities()
	{
		int count = m_opaqueEnts.size();
		for( int i = 0; i< count ; i++ )
		{
			for( int j = i+1; j< count; j++ )
			{
				if( CompareMaterials( m_opaqueEnts[i], m_opaqueEnts[j] ) == 0 )
				//if( m_opaqueEnts[i]->getMaterialFile() == m_opaqueEnts[j]->getMaterialFile() )
				{
					cMesh* temp = m_opaqueEnts[j];
					m_opaqueEnts[j] = m_opaqueEnts[i+1];
					m_opaqueEnts[i+1] = temp;
				}
			}
		}

		for( int i= 0; i<count; i++ )
		{
			for( int j= i+1; j< count; j++ )
			{
				if( m_opaqueEnts[i]->getEffect() == m_opaqueEnts[j]->getEffect() )
				{
					cMesh* temp = m_opaqueEnts[j];
					m_opaqueEnts[j] = m_opaqueEnts[i+1];
					m_opaqueEnts[i+1] = temp;
				}
			}
		}
	}

	void cSceneHandler::sortTransEntities()
	{
		for( unsigned int i=0; i< m_transEnts.size() ; i++ )
		{
			float prevMax = 0.0f;
			unsigned int indexSwap = 0;
			for( unsigned int j=i; j<m_transEnts.size(); j++ )
			{
				D3DXVECTOR3 dist= m_transEnts[j]->getPosition() - cTPSCamera::getInstance()->getPosition();
				float distVal = dist.x* dist.x + dist.y* dist.y + dist.z* dist.z;

				if( distVal > prevMax )
				{
					prevMax = distVal;
					indexSwap = j;
				}
			}

			cMesh* tempEnt = m_transEnts[i];
			m_transEnts[i]= m_transEnts[indexSwap];
			m_transEnts[indexSwap] = tempEnt;
		}
	}

	void cSceneHandler::Draw()
	{

		for( unsigned int itr=0; itr< m_sprites.size(); itr++ )
		{
			if( m_sprites[itr]->GetDrawSprite() )
				m_sprites[itr]->Draw();
		}

		for( unsigned int itr=0; itr< m_texts.size(); itr++ )
		{
			if( m_texts[itr]->GetDrawText() )
				m_texts[itr]->Draw();
		}

		for( unsigned int itr = 0; itr < m_triggerBoxes.size(); itr++ )
		{
			m_triggerBoxes[itr]->Draw(m_direct3dDevice);
		}

		for( unsigned int itr = 0; itr < m_debugLines.size(); itr++ )
		{
			m_debugLines[itr]->Draw( m_direct3dDevice );
		}

		//if( Optimus::DebugConsole::getInstance()->GetDebugConsoleState() )
		Optimus::DebugConsole::getInstance()->DrawDebugConsole();
		
		for( unsigned int i = 0 ; i < m_opaqueEnts.size() ; i++ )
		{
			D3DXMATRIX viewToProjected;
			D3DXMatrixPerspectiveFovLH(&viewToProjected, m_fovAngle, 1.78f, 0.1f, 5000.0f );

			if( i != 0 )
			{
				int effectCompare = strcmp( m_opaqueEnts[i-1]->getEffects().c_str(), m_opaqueEnts[i]->getEffects().c_str() );
				if( effectCompare != 0 )
				{
					
					m_opaqueEnts[i]->loadEffect();
				}

				int materialCompare = CompareMaterials( m_opaqueEnts[i-1], m_opaqueEnts[i] );
				//int materialCompare = strcmp( m_opaqueEnts[i-1]->getMaterialFile().c_str(), m_opaqueEnts[i]->getMaterialFile().c_str() );
				if( materialCompare != 0 )
				{
					
					m_opaqueEnts[i]->loadTexture();
					
					m_opaqueEnts[i]->LoadTransforms(cTPSCamera::getInstance()->GetWorldToViewTransform(), viewToProjected, m_lighting);
					m_opaqueEnts[i]->Update();
					m_opaqueEnts[i]->Draw();
					
				}
				else
				{
					
					m_opaqueEnts[i]->LoadTransforms(cTPSCamera::getInstance()->GetWorldToViewTransform(), viewToProjected, m_lighting);
					m_opaqueEnts[i]->Update();
					m_opaqueEnts[i]->Draw();
					
				}
			}
			else
			{
				
				m_opaqueEnts[i]->loadEffect();
				
				m_opaqueEnts[i]->loadTexture();
				
				m_opaqueEnts[i]->LoadTransforms(cTPSCamera::getInstance()->GetWorldToViewTransform(), viewToProjected, m_lighting);
				m_opaqueEnts[i]->Update();
				m_opaqueEnts[i]->Draw();

			}		
		}

		for( unsigned int i = 0 ; i < m_transEnts.size() ; i++ )
		{
			D3DXMATRIX viewToProjected;
			D3DXMatrixPerspectiveFovLH(&viewToProjected, D3DX_PI / 4.0f, 1.8f, 0.1f, 100.0f);

			if( i != 0 )
			{
				int effectCompare = strcmp( m_transEnts[i-1]->getEffect()->getRenderState().c_str(), m_transEnts[i]->getEffect()->getRenderState().c_str() );
				if( effectCompare != 0 )
				{
					
					m_transEnts[i]->loadEffect();
				}

				int materialCompare = CompareMaterials( m_transEnts[i-1], m_transEnts[i] );
				//int materialCompare = strcmp( m_transEnts[i-1]->getMaterialFile().c_str(), m_transEnts[i]->getMaterialFile().c_str() );
				if( materialCompare != 0 )
				{
					
					m_transEnts[i]->loadTexture();
					
					m_transEnts[i]->LoadTransforms(cTPSCamera::getInstance()->GetWorldToViewTransform(), viewToProjected, m_lighting);
					m_transEnts[i]->Update();
					m_transEnts[i]->Draw();
					
				}
				else
				{
					
					m_transEnts[i]->LoadTransforms(cTPSCamera::getInstance()->GetWorldToViewTransform(), viewToProjected, m_lighting);
					m_transEnts[i]->Update();
					m_transEnts[i]->Draw();
					
				}
			}
			else
			{
				m_transEnts[i]->loadEffect();
			
				m_transEnts[i]->loadTexture();
				
				m_transEnts[i]->LoadTransforms(cTPSCamera::getInstance()->GetWorldToViewTransform(), viewToProjected, m_lighting);
				m_transEnts[i]->Update();
				m_transEnts[i]->Draw();
			}		
		}
		DeleteDebugLines();
		m_debugLines.clear();
	}

	bool cSceneHandler::parseDynamicFile( const char* i_fileName )
	{
		std::vector<Optimus::Vector3> _pos, _size;
		std::vector<int> _meshIndices;
		std::ifstream dynamicFile;
		int _dynamicEntityNumber, _tempID;
		Optimus::Vector3 _tempPos, _tempSize;
		dynamicFile.open( i_fileName, std::ios::binary | std::ios::in );
		if( dynamicFile.peek() == std::ifstream::traits_type::eof() )
		{
			dynamicFile.close();
			return false;
		}
		dynamicFile.read(reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) ); //Number of trigger boxes
		if( _dynamicEntityNumber < 0 )
		{
			dynamicFile.close();
			return false;
		}
		for( int i = 0; i < _dynamicEntityNumber; i++ ) //TriggerBox sizes and positions
		{
			dynamicFile.read(reinterpret_cast<char*>(&_tempPos), sizeof( Optimus::Vector3 ) );
			dynamicFile.read(reinterpret_cast<char*>(&_tempSize), sizeof( Optimus::Vector3 ) );
			_pos.push_back( _tempPos );
			_size.push_back( _tempSize );
		}
		dynamicFile.read(reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) ); //Number of indices of Dynamic Meshes
		if( _dynamicEntityNumber < 0 )
		{
			dynamicFile.close();
			return false;
		}
		for( int i = 0; i < _dynamicEntityNumber; i++ ) //Dynamic mesh Indices
		{
			dynamicFile.read(reinterpret_cast<char*>(&_tempID), sizeof( int ) );
			_meshIndices.push_back(_tempID);
		}
		//Mark these meshes as dynamic
		for( unsigned int i = 0; i< _meshIndices.size(); i++ )
		{
			for( unsigned int j = 0; j < m_opaqueEnts.size(); j++ )
			{
				if( _meshIndices[i] == j )
				{
					m_opaqueEnts[j]->setDynamic( true );
					continue;
				}
			}
		}
		if( _pos.size() != _size.size() )
		{
			dynamicFile.close();
			return false;
		}
		unsigned int neighbourCount;
		std::map<int, std::vector<Optimus::Vector3>> _tempNeighbourMap;
		std::vector<Optimus::Vector3> _tempNodePosition;
		dynamicFile.read( reinterpret_cast<char*>(&_dynamicEntityNumber), sizeof( int ) ); //Number of WayPoints
		for( unsigned int i = 0; i < _dynamicEntityNumber; i++ )
		{
			Optimus::Vector3 _tempWPPos, _tempPos2;
			std::vector<Optimus::Vector3> _tempNeighbourPos;
			dynamicFile.read( reinterpret_cast<char*>( &_tempWPPos ), sizeof( Optimus::Vector3 ) ); //Position of WayPointNode
			_tempNodePosition.push_back( _tempWPPos );
			dynamicFile.read( reinterpret_cast<char*>( &neighbourCount ), sizeof( unsigned int ) ); //Number of Neighbours
			for( unsigned int j = 0; j < neighbourCount; j++ )
			{
				dynamicFile.read( reinterpret_cast<char*>( &_tempPos2 ), sizeof( Optimus::Vector3 ) ); //Position of Neighbours
				_tempNeighbourPos.push_back( _tempPos2 );
			}
			_tempNeighbourMap.insert( std::pair<int, std::vector<Optimus::Vector3>>(i, _tempNeighbourPos) );
			_tempNeighbourPos.clear();
		}
		for( unsigned int i = 0; i < _pos.size(); i++ ) //Add the TriggerBoxes
		{
			AddTriggerBox( _pos[i], _size[i] );
		}
		//Read in Hard Surfaces
		int hardSurfaceCount;
		std::vector<int> hardSurfaceIDs;
		dynamicFile.read( reinterpret_cast<char*>(&hardSurfaceCount), sizeof( int ) ); //Number of Hard Surfaces
		for( unsigned int i = 0; i < hardSurfaceCount; i++ )
		{
			int temp;
			dynamicFile.read(reinterpret_cast<char*>(&temp), sizeof( int ) );
			hardSurfaceIDs.push_back( temp );
		}
		//Mark these meshes as hard surfaces
		for( unsigned int i = 0; i< hardSurfaceIDs.size(); i++ )
		{
			for( unsigned int j = 0; j < m_opaqueEnts.size(); j++ )
			{
				if( hardSurfaceIDs[i] == j )
				{
					m_opaqueEnts[j]->m_hardSurface = true;
					continue;
				}
			}
		}
		dynamicFile.close();
		//Create WayPoints
		for( unsigned int i = 0; i < _dynamicEntityNumber; i++ )
		{
			WayPointNode* _tempWayPoint = OPT_NEW WayPointNode( _tempNodePosition[i], i );
			_tempWayPoint->GetTriggerBox()->SetDrawable( false );
			WayPointGraph::GetInstance()->AddWayPoint( _tempWayPoint );
		}
		for( unsigned int i = 0; i < _tempNeighbourMap.size(); i++ )
		{
			for( unsigned int j = 0; j < _tempNeighbourMap[i].size(); j++ )
			{
				WayPointGraph::GetInstance()->m_listNodes[i]->AddNeighbour( WayPointGraph::GetInstance()->GetNeighbour( _tempNeighbourMap[i][j] ) );
			}
		}
		return true;
	}

	bool cSceneHandler::parseSceneFile( const char* i_fileName )
	{
		char c= 'a';
		std::string tempString;
		float temp1, temp2, temp3, temp4;		
		std::ifstream scenefile;
		const char* name = i_fileName;
		unsigned int _accessCount;
		std::vector<std::string> fileNames;
		int value = 0;
		int _counter = 0, _current = 0;
		scenefile.open (name);

		while( !scenefile.eof() )
		{
			scenefile >> c;
			if( c == '>' )
			{
				scenefile >> _accessCount;
			}
			if( c == '-' )
			{
				for( unsigned int i = 0; i <_accessCount; i++ )
				{
					scenefile >>value;
					m_entityCount.push_back(value);
				}
			}
			else if( c == ':' )
			{
				for( unsigned int i = 0; i < m_entityCount.size(); i++ )
				{
					_counter += m_entityCount[i];
				}
				for( unsigned int i = 0; i < _accessCount; i++ )
				{
					std::string tempName;
					scenefile>> tempName;
					fileNames.push_back( tempName );
				}
			}
			else if( c == '_' )
			{
				scenefile>>temp1>>temp2>>temp3;
				cTPSCamera::getInstance()->setPosition( temp1,temp2,temp3 );
			}
			else if( c == '=' )
			{
				scenefile>>temp1>>temp2>>temp3;
				m_lighting->setPosition( D3DXVECTOR3( temp1, temp2, temp3 ) );
				scenefile>>temp1>>temp2>>temp3>>temp4;
				m_lighting->setParams( D3DXVECTOR4( temp1, temp2, temp3, temp4 ) );
				scenefile>>temp1;
				m_lighting->setAttFac( temp1 );
			}
		}
		scenefile.close();
		for( unsigned int i = 0 ; i < _accessCount; i++ )
		{
			for( unsigned int j=0; j < m_entityCount[i]; j++ )
			{
				int solid = 1;
				m_entities.push_back( OPT_NEW cMesh() );

				if( j != 0 )
				{
					m_entities[j]->setData( m_entities[j-1]->getData() );
				}
				m_entities[_current]->Initialize( m_direct3dDevice );
				m_entities[_current]->parseFiles( fileNames[i].c_str(), j );
				m_entities[_current]->setSolid( solid );
				if( _current < _counter )
				{
					_current++;
				}
			}
		}
		divideOpaqueTrans();
		sortOpaqueEntities();
		sortTransEntities();
		/*Optimus::DebugConsole::getInstance()->AddButton( "Renderer", &RendererMenu );
		Optimus::DebugConsole::getInstance()->AddCheckbox( "Renderer/Wireframe", &WireFrameIt );*/
		DebugConsole::getInstance()->AddButton( "CamMov", &CameraMovement );
		DebugConsole::getInstance()->AddCheckbox( "CamMov/FreeCam", &FreeFormCamera );
		return true;
	}

	void cSceneHandler::GoToWireframe( bool i_boolValue )
	{
		m_isWireframe = i_boolValue;
		for( unsigned int i =0; i< m_opaqueEnts.size(); i++ )
		{
			m_opaqueEnts[i]->setSolid( m_isWireframe );
		}
		for( unsigned int i =0; i< m_transEnts.size(); i++ )
		{
			m_transEnts[i]->setSolid( m_isWireframe );
		}
	}

	void cSceneHandler::divideOpaqueTrans()
	{
		for( unsigned int i =0; i< m_entities.size(); i++ )
		{
			if( m_entities[i]->getEffect()->getRenderState() == "NULL" ||  m_entities[i]->getEffect()->getRenderState() == "BIN" )
			{
				m_opaqueEnts.push_back( m_entities[i] );
			}
			else
			{
				m_transEnts.push_back( m_entities[i] );
			}
		}
	}

	int cSceneHandler::CompareMaterials( cMesh* i_ent1, cMesh* i_ent2 )
	{
		int textureCheck, effectCheck;

		effectCheck = strcmp( i_ent1->getEffects().c_str(), i_ent1->getEffects().c_str() );

		textureCheck = strcmp( i_ent1->getTexture().c_str(), i_ent2->getTexture().c_str() );

		if( effectCheck == 0 && textureCheck == 0 )
			return 0;
		else
			return 1;
	}
}